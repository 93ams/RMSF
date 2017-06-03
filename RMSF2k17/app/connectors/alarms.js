import RestClient from 'react-native-rest-client';

export default class AlarmsConnector extends RestClient {
  constructor () {
    super('http://6e7fe579.ngrok.io');
  }

  list() {
    return this.GET('/alarms').then(res => res.alarms);
  }

  get(id) {
    return this.GET('/alarms/'+id).then(res => res.alarm);
  }
};
