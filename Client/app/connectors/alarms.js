import RestClient from 'react-native-rest-client';

export default class AlarmsConnector extends RestClient {
  constructor () {
    super('http://a912309f.ngrok.io');
  }

  list() {
    return this.GET('/alarms')
    .then(res => res.alarms)
    .catch(err => {
      console.log('Error: ' + err);
      throw err;
    });
  }

  get(id) {
    return this.GET('/alarms/'+id)
    .then(res => res.alarm)
    .catch(err => {
      console.log('Error: ' + err);
      throw err;
    });
  }
};
