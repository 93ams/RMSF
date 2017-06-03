const hapi = require('hapi');
const server = new hapi.Server();
const socket = require('./socket');

server.connection({ port: 8080, labels: ['api'] });
server.connection({ port: 8000, labels: ['socket'] });

socket.connect(server.select('socket').listener);
server.select('api').route(require('./routes'));

module.exports = server;
