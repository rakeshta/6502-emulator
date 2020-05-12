//
//  index.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React      from 'react';
import ReactDOM   from 'react-dom';

import App        from './App';

import './style/root.scss';


ReactDOM.render(<App/>, document.getElementById('__app'));

if (module.hot) {
    module.hot.accept();
}
