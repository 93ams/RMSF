const handlers = require('./handlers');

const create = {
  method: 'POST',
  path: '/alarms',
  handler: handlers.create
};

const list = {
  method: 'GET',
  path: '/alarms',
  handler: handlers.list
};

const get = {
  method: 'GET',
  path: '/alarms/{id}',
  handler: handlers.get
};

module.exports = [
  create,
  list,
  get
];
