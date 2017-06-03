import React, { Component } from 'react';
import { View, Text, StyleSheet } from 'react-native';
import { Actions } from 'react-native-router-flux';

export default class Home extends Component {
  render() {
    return (
      <View>
        <Text
          style={styles.alarms}
          onPress={() => Actions.alarms()}>
          Alarms
        </Text>
      </View>
    )
  }
}

const styles = StyleSheet.create({
  alarms: {
    paddingTop: 62,
    color: 'blue',
    fontSize: 30
  }
});
