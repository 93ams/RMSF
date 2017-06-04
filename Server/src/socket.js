const gcm = require('node-gcm');
const tokens = require('./tokens');

var io = null;
var connections = [];

function removeConnection(socket){
  const id = connections.indexOf(socket);
  if(id != -1) connections.splice(id, 1);
}

const connect = function(listener){
  io = require('socket.io')(listener);
  io.on('connection', function (socket) {
      var localToken;
      socket.on('register', ({ token }) => {
        tokens.addToken(token);
        localToken = token;
      });

      socket.on('disconnect', () => { removeConnection(socket); });
      connections.push(socket);
  });
}

const getIO = function(){ return io; }

const broadcastAlarm = function(data){
  connections.forEach((connection) => connection.emit('newAlarm', data));
}

module.exports = {
  getIO,
  connect,
  broadcastAlarm
}
