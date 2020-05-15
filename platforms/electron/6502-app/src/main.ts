//
//  main.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import { app, BrowserWindow } from 'electron';
import installExtension, {
    REACT_DEVELOPER_TOOLS,
}                             from 'electron-devtools-installer';
import contextMenu            from 'electron-context-menu';
import windowStateKeeper      from 'electron-window-state';
import isDev                  from 'electron-is-dev';

import { background }         from './style/theme';


declare const MAIN_WINDOW_WEBPACK_ENTRY: string;


// Handle creating/removing shortcuts on Windows when installing/uninstalling.
if (require('electron-squirrel-startup')) { // eslint-disable-line global-require
    app.quit();
}

// set-up context menu for dev-env
if (isDev) {
    contextMenu();
}

// create window on ready
app.on('ready', () => {

    // load previous window state or use defaults
    const windowState = windowStateKeeper({defaultWidth: 1024, defaultHeight: 768});

    // create the browser window
    const win = new BrowserWindow({
        x:                     windowState.x,
        y:                     windowState.y,
        width:                 windowState.width,
        height:                windowState.height,
        minWidth:              1024,
        minHeight:             768,
        titleBarStyle:        'hidden',
        trafficLightPosition: {x: 12, y: 31}, // Magic number calculated to match app top-nav layout
        backgroundColor:       background.chrome,
        webPreferences:       {
            nodeIntegration:   true,
        },
    });

    // let window state manager listen to window size changes
    windowState.manage(win);

    // load index.html
    win.loadURL(MAIN_WINDOW_WEBPACK_ENTRY);

    // open dev-tools in dev-env & install extensions
    if (isDev) {
        win.webContents.openDevTools();
        installExtension(REACT_DEVELOPER_TOOLS);
    }
});

// Quit when all windows are closed.
app.on('window-all-closed', app.quit);
