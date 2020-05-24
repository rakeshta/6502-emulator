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


// set platform name on document for platform dependent styles
document.documentElement.setAttribute('data-platform', process.platform);

// render app
ReactDOM.render(<WindowRouter/>, document.getElementById('__app'));

// enable hot reloading
if (module.hot) {
    module.hot.accept();
}
