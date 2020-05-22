//
//  start.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 22 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import { app, Menu }          from 'electron';
import installExtension, {
    REACT_DEVELOPER_TOOLS,
}                             from 'electron-devtools-installer';
import contextMenu            from 'electron-context-menu';
import isDev                  from 'electron-is-dev';

import appMenu                from './menu';
import controller             from './controller';


// set-up --------------------------------------------------------------------------------------------------------------

// Handle creating/removing shortcuts on Windows when installing/uninstalling.
if (require('electron-squirrel-startup')) {
    app.quit();
}

// set-up context menu for dev-env
if (isDev) {
    contextMenu();
}

// set main-menu
Menu.setApplicationMenu(appMenu);


// app events ----------------------------------------------------------------------------------------------------------

// install dev-tools & create window when app is ready
app.on('ready', () => {

    // install dev-tools if in dev-env
    if (isDev) {
        installExtension(REACT_DEVELOPER_TOOLS);
    }

    // show welcome window
    controller.welcome.show();
});

// quit when all windows are closed.
app.on('window-all-closed', app.quit);
