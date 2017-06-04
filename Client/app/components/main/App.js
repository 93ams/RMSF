import { createStore, applyMiddleware } from 'redux';
import React, { Component } from 'react';
import { Provider } from 'react-redux';
import reduxThunk from 'redux-thunk';

import reducers from '../../reducers';
import AppRouter from './AppRouter';

import socket from '../../connectors/socket';

const createStoreWithMiddleware = applyMiddleware(reduxThunk)(createStore);
const store = createStoreWithMiddleware(reducers);

export default class App extends Component {
  render() {
    return (
      <Provider store={store}>
        <AppRouter />
      </Provider>
    )
  }
}
