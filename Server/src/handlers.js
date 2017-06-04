const gcm = require('node-gcm');
const socket = require('./socket');
const tokens = require('./tokens');
const io = socket.getIO();

var sender = new gcm.Sender('AIzaSyCra3r9CUqgJLTALPKzvkDBJwkvqHKfFIA'); //TODO: hide
var rfid_ids = ['7AD816CB', '967BD08C'];

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
  data.id = alarms.length;
  data.date = new Date(data.date);
  alarms.push(data);
  //socket.broadcastAlarm(data);
  sender.send(new gcm.Message({ data }),
    { registrationTokens: tokens.getTokens() },
    function (err, response) {
    if (err) console.error(err);
    else console.log(response);
  });
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

const config = function(req, res){
  const data = parseData(req.payload);
  console.log('New Device');
  console.log(data);
  res.json({
    '76F46': {
    	 'downlinkData': rfid_ids.toString()
     }
  });
  //res(rfid_ids.toString());
}

module.exports = {
  config,
  create,
  list,
  get
};
