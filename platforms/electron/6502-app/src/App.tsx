//
//  App.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React     from 'react';

import AppFrame  from './components/layout/AppFrame';
import Editor    from './editor/Editor';


const App = React.memo(() => (
    <AppFrame>
        <Editor/>
    </AppFrame>
));

App.displayName = 'App';

export default App;
