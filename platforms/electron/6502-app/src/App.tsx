//
//  App.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React     from 'react';

import TitleBar  from './components/layout/TitleBar';
import AppFrame  from './components/layout/AppFrame';


const App = React.memo(() => (
    <AppFrame>
        <TitleBar title='RT 6502 Emulator'/>
        <div className='m-3'>
            <h1>Hello React Electron App</h1>
            <button className='btn btn-primary'>Click Me</button>
        </div>
    </AppFrame>
));

App.displayName = 'App';

export default App;
