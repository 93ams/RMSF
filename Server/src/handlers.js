const socket = require('./socket');
const io = socket.getIO();

var alarms = [{
  id: 1,
  name: "NATAL",
  date: new Date()
}, {
  id: 2,
  name: "PASCOA",
  date: new Date()
}, {
  id: 69,
  name: "MAMAS",
  date: new Date()
}];

function getAlarm(id){
  return alarms.find((alarm) => { return alarm.id == id });
}

function parseData(data){
  if(typeof(data) == 'string'){
    return JSON.parse(data);
  } else if(typeof(data) == 'object'){
    return data;
  }
  throw 'Error: Invalid data type.';
}

const create = function(req, res){
  const data = parseData(req.payload);
  console.log(data);
  alarms.push(data);
  socket.broadcastAlarm(data);
  res().code(201);
}

const list = function(req, res){
  res({ alarms });
}

const get = function(req, res){
  const id = req.params.id;
  const alarm = getAlarm(id);
  res({ alarm });
}

module.exports = {
  create,
  list,
  get
};
