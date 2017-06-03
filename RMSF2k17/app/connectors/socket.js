import { DeviceEventEmitter } from 'react-native';
import PushNotification from 'react-native-push-notification';
import { Actions } from 'react-native-router-flux';
import io from 'socket.io-client/dist/socket.io';

PushNotification.configure({
    onNotification: ({ data }) => { Actions.alarm(data.id); }
});

const socket = io('http://f84a0e7f.ngrok.io');

socket.on('connect', () => { console.log('Connect'); });

socket.on('newAlarm', (data) => {
  PushNotification.localNotification({
    ticker: "My Notification Ticker", // (optional)
    largeIcon: "ic_launcher", // (optional) default: "ic_launcher"
    smallIcon: "ic_notification", // (optional) default: "ic_notification" with fallback for "ic_launcher"
    bigText: "I am sending you this message, so you can get your ass home asap and avoid worse consequences, be sure to pay me a dinner sometime!",
    subText: "Hurry the fuck up!",
    color: "red", // (optional) default: system default
    vibrate: true, // (optional) default: true
    vibration: 300, // vibration length in milliseconds, ignored if vibrate=false, default: 1000

    /* iOS and Android properties */
    title: "New Alarm", // (optional, for iOS this is only used in apple watch, the title will be the app name on other iOS devices)
    message: "Your house is being robbed and your family raped", // (required)
    soundName: 'default', // (optional) Sound to play when the notification is shown. Value of 'default' plays the default sound. It can be set to a custom sound such as 'android.resource://com.xyz/raw/my_sound'. It will look for the 'my_sound' audio file in 'res/raw' directory and play it. default: 'default' (default sound is played)
    data: data
  });
});
socket.on('disconnect', () => {console.log('Disconnect'); });

export default socket;
