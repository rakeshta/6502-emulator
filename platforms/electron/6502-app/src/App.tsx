//
//  App.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React     from 'react';

import TitleBar  from './components/layout/TitleBar';


const App = React.memo(() => (
    <React.Fragment>
        <TitleBar title='RT 6502 Emulator'/>
        <h1>Hello React Electron App</h1>
    </React.Fragment>
));

App.displayName = 'App';

export default App;
