var io = null;
var connections = [];

const connect = function(listener){
  io = require('socket.io')(listener);
  io.on('connection', function (socket) {
      console.log('New connection!');
      socket.emit("newAlarm", { "id": 69 });
      connections.push(socket);
  });
}

const getIO = function(){ return io; }

const broadcastAlarm = function(data){
  connections.forEach((connection) => connection.emit('newAlarm', data));
}

module.exports = {
  getIO,
  connect
}
