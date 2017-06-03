import React, { Component } from 'react';
import {
  StyleSheet,
  FlatList,
  View
} from 'react-native';
import { Actions } from 'react-native-router-flux';
import { List, ListItem } from "react-native-elements";
import Connector from '../../connectors/alarms';

export default class Alarms extends Component {
  constructor(props) {
    super(props);
    const connector = new Connector();

    this.state = { alarms: [] };

    connector.list()
    .then((alarms) => this.setState({ alarms }))
    .catch((err) => {
      Actions.home();
      alert('Unnable to get alarms; Error: ', err);
    });
  }

  render() {
    return (
      <View>
        <List containerStyle={{ paddingTop: 40, borderBottomWidth: 0 }}>
          <FlatList
            data={this.state.alarms}
            keyExtractor={item => item.id}
            renderItem={({item}) => (
              <ListItem
                containerStyle={{ borderBottomWidth: 0 }}
                title={item.name}
                onPress={() => Actions.alarm(item.id)} />
            )}
          />
        </List>
      </View>
    )
  }
}
