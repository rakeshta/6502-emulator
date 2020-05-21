//
//  index.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React         from 'react';
import ReactDOM      from 'react-dom';

import WindowRouter  from './windows/WindowRouter';

import './index.scss';


const div = document.getElementById('__app');
if (div) {
    div.className = 'platform-' + process.platform;
}

ReactDOM.render(<WindowRouter/>, div);

if (module.hot) {
    module.hot.accept();
}
