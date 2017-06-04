import { DeviceEventEmitter } from 'react-native';
import PushNotification from 'react-native-push-notification';
import { Actions } from 'react-native-router-flux';
import io from 'socket.io-client/dist/socket.io';

const socket = io('http://1fb97ff1.ngrok.io');

function sendNotification(id){
  PushNotification.localNotification({
    largeIcon: "ic_launcher",
    smallIcon: "ic_notification",
    bigText: "I am sending you this message, so you can get your ass home asap and avoid worse consequences, be sure to pay me a dinner sometime!",
    subText: "Hurry the fuck up!",
    vibrate: true,
    vibration: 1000,
    title: "New Alarm",
    message: "Your house is being robbed and your family raped",
    soundName: 'default',
    data: id
  });
}

PushNotification.configure({
  onRegister: (token) => { socket.emit('register', token); },
  onNotification: (notification) => {
    const fromGoogle = notification['google.message_id'];
    const id = fromGoogle ? notification.id : notification.data.id;

    if(fromGoogle) { sendNotification({ id }); }
    else { Actions.alarm(id); }
  },
  senderID: "398333798989"
});

socket.on('connect', () => { console.log('Connect'); });

socket.on('newAlarm', ({ id }) => {
  console.log('ID: '+id);
  sendNotification({ id });
});

socket.on('disconnect', () => {console.log('Disconnect'); });

export default socket;
