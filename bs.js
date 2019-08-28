// beanstalk client
// protocol: https://raw.githubusercontent.com/beanstalkd/\
//    beanstalkd/master/doc/protocol.txt

const PAYLOAD_COMMANDS = ["RESERVED", "FOUND", "OK"];

function _parseYAMLList(data) {
  if (data.substring(0, 4) != "---\n") return data;
  return data.substring(4, data.length - 1).split('\n')
    .map(x => x.substring(2));
}

function _parseYAMLDict(data) {
  if (data.substring(0, 4) != "---\n") return data;
  const ret = {};
  const l = data.substring(4).split('\n');
  for (let i = 0; i < l.length; ++i) {
    const line = l[i];
    const idx = line.indexOf(':');
    if (idx == -1) continue;
    const k = line.substring(0, idx);
    const v = line.substring(idx + 2, line.length);
    ret[k] = isNaN(v) ? v : parseInt(v);
  }

  return ret;
}

export default class BS {
  constructor(url) {
    this.socket = new WebSocket(url);
    this.socket.binaryType = "arraybuffer";
    this.connected = false;

    this.ready = new Promise((accept, reject) => {
      this.socket.addEventListener("open", _ev => {
        this.connected = true;
        accept(this);
      });

      this.socket.addEventListener("error", ev => {
        this.connected = false;
        reject(ev);
      });
    });

    let buffer = "";
    this.pending = [];
    this.pendingDataSize = 0;
    this.pendingCommand = null;

    this.socket.addEventListener("message", ev => {
      buffer += String.fromCharCode.apply(null, new Uint8Array(ev.data));

      while(buffer.length) {
        if (this.pendingDataSize) {
          if (buffer.length < this.pendingDataSize + 2) break;

          if (buffer[this.pendingDataSize] != '\r' ||
              buffer[this.pendingDataSize + 1] != '\n') {
            // eslint-disable-next-line no-console
            console.error("Missing data on buffer:", buffer);
          }
          const data = buffer.substring(0, this.pendingDataSize);
          buffer = buffer.substring(this.pendingDataSize + 2);

          this.pending.shift()(...this.pendingCommand, data);
          this.pendingCommand = null;
          this.pendingDataSize = 0;
          continue;
        }

        const index = buffer.indexOf('\r\n');
        if (index == -1) {
          break;
        }

        const head = buffer.substring(0, index);
        buffer = buffer.substring(index + 2);
        const cmd = head.split(' ');

        if (PAYLOAD_COMMANDS.indexOf(cmd[0]) != -1) {
          this.pendingCommand = cmd.slice(0, cmd.length - 1);
          this.pendingDataSize = Number(cmd[cmd.length - 1]);
        } else {
          this.pending.shift()(...cmd);
        }
      }
    });

    this.socket.addEventListener("close", _ev => {
      this.connected = false;
    });
  }

  async _buildr(good, answer, bad = []) {
    if (!this.connected) return;
    return await new Promise((acc, rej) => {
      this.pending.push((...cmd) => {
        const errors = ["OUT_OF_MEMORY", "INTERNAL_ERROR", "BAD_FORMAT",
          "UNKNOWN_COMMAND"].concat(bad);

        if (errors.some(err => err == cmd[0])) {
          return rej(cmd);
        }

        if (cmd[0] == good) {
          return acc(answer(...cmd.slice(1)));
        }

        return rej("INVALID_RESPONSE", ...cmd);
      });
    });
  }

  _send(msg, data = null) {
    if (!this.connected) return;
    if (data) {
      this.socket.send(`${msg} ${data.length}\r\n${data}\r\n`);
    } else {
      this.socket.send(msg + '\r\n');
    }
  }

  async put(payload, pri = 0, delay = 0, ttr = 60) {
    let data = payload;
    if (typeof payload === 'object') {
      data = JSON.stringify(payload);
    }

    this._send(`put ${pri} ${delay} ${ttr}`, data);
    return this._buildr("INSERTED", id => ({id: id}),
      ["BURIED", "EXPECTED_CRLF", "JOB_TOO_BIG", "DRAINING"]);
  }

  async use(tube) {
    this._send(`use ${tube}`);
    return this._buildr("USING", tube => ({tube: tube}));
  }

  async reserve(timeout = -1) {
    if (timeout == -1) {
      this._send(`reserve`);
    } else {
      this._send(`reserve-with-timeout ${timeout}`);
    }
    return this._buildr("RESERVED", (id, data) => ({id: id,
      data: JSON.parse(data)}),
    ["DEADLINE_SOON", "TIMED_OUT"]);
  }

  async delete(id) {
    this._send(`delete ${id}`);
    return this._buildr("DELETED", () => {},
      ["NOT_FOUND"]);
  }

  async release(id, pri = 0, delay = 0) {
    this._send(`release ${id} ${pri} ${delay}`);
    return this._buildr("RELEASED", () => {},
      ["BURIED", "NOT_FOUND"]);
  }

  async bury(id, pri = 0) {
    this._send(`bury ${id} ${pri}`);
    return this._buildr("BURIED", () => {},
      ["NOT_FOUND"]);
  }

  async touch(id) {
    this._send(`touch ${id}`);
    return this._buildr("TOUCHED", () => {},
      ["NOT_FOUND"]);
  }

  async watch(tube) {
    this._send(`watch ${tube}`);
    return this._buildr("WATCHING", tube => ({tube: tube}));
  }

  async ignore(tube) {
    this._send(`ignore ${tube}`);
    return this._buildr("WATCHING", tube => ({tube: tube}),
      ["NOT_IGNORED"]);
  }

  async peek(id) {
    this._send(`peek ${id}`);
    return this._buildr("FOUND", (id, data) => ({id: id,
      data: JSON.parse(data)}),
    ["NOT_FOUND"]);
  }

  async peekReady() {
    this._send(`peek-ready`);
    return this._buildr("FOUND", (id, data) => ({id: id,
      data: JSON.parse(data)}),
    ["NOT_FOUND"]);
  }

  async peekDelayed() {
    this._send(`peek-delayed`);
    return this._buildr("FOUND", (id, data) => ({id: id,
      data: JSON.parse(data)}),
    ["NOT_FOUND"]);
  }

  async peekBuried() {
    this._send(`peek-buried`);
    return this._buildr("FOUND", (id, data) => ({id: id,
      data: JSON.parse(data)}),
    ["NOT_FOUND"]);
  }

  async kick(bound) {
    this._send(`kick ${bound}`);
    return this._buildr("KICKED", bound => ({bound: bound}));
  }

  async kickJob(id) {
    this._send(`kick-job ${id}`);
    return this._buildr("KICKED", () => {},
      ["NOT_FOUND"]);
  }

  async statsJob(id) {
    this._send(`stats-job ${id}`);
    return this._buildr("OK", stats => ({stats: _parseYAMLDict(stats)}),
      ["NOT_FOUND"]);
  }

  async statsTube(tube) {
    this._send(`stats-tube ${tube}`);
    return this._buildr("OK", stats => ({stats: _parseYAMLDict(stats)}),
      ["NOT_FOUND"]);
  }

  async stats() {
    this._send(`stats`);
    return this._buildr("OK", stats => ({stats: _parseYAMLDict(stats)}),
      ["NOT_FOUND"]);
  }

  async listTubes() {
    this._send(`list-tubes`);
    return this._buildr("OK", tubes => ({tubes: _parseYAMLList(tubes)}),
      ["NOT_FOUND"]);
  }

  async listTubeUsed() {
    this._send(`list-tube-used`);
    return this._buildr("USING", tube => ({tube: tube}),
      ["NOT_FOUND"]);
  }

  async listTubesWatched() {
    this._send(`list-tubes-watched`);
    return this._buildr("OK", tubes => ({tubes: _parseYAMLList(tubes)}),
      ["NOT_FOUND"]);
  }

  async quit() {
    this._send(`quit`);
    this.socket.close();
    this.connected = false;
  }

  async pauseTube(tube, delay) {
    this._send(`pause-tube ${tube} ${delay}`);
    return this._buildr("PAUSED", () => {}, ["NOT_FOUND"]);
  }
}
