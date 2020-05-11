//
//  index.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Pixxa LLC. All rights reserved.
//

import React            from 'react';
import ReactDOM         from 'react-dom';

import './index.css';


ReactDOM.render(<h1>Hello Electron React</h1>, document.getElementById('__app'));

if (module.hot) {
    module.hot.accept();
}
