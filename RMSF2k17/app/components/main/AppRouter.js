import React, { Component } from 'react';
import { connect } from 'react-redux';
import { Actions, Scene, Router } from 'react-native-router-flux';

import Home from '../pages/Home';
import Alarm from '../pages/Alarm';
import Alarms from '../pages/Alarms';

const scenes = Actions.create(
  <Scene key="root">
    <Scene key="home" component={Home} initial />
    <Scene key="alarm" component={Alarm} title="Alarm" />
    <Scene key="alarms" component={Alarms} title="Alarms" />
  </Scene>
);

export default connect()(() => <Router scenes={scenes} />);
