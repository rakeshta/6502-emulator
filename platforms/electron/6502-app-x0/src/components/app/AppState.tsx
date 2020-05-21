//
//  AppState.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 20 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React     from 'react';


// context AppState ----------------------------------------------------------------------------------------------------

export interface AppState {
    activeTabKey?:  string;
}

const AppState = React.createContext<AppState>({
    activeTabKey:   undefined,
});


//
