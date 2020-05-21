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
import isDev                  from 'electron-is-dev';

import { background }         from './styles/theme';


declare const MAIN_WINDOW_WEBPACK_ENTRY: string;


// set-up --------------------------------------------------------------------------------------------------------------

// Handle creating/removing shortcuts on Windows when installing/uninstalling.
if (require('electron-squirrel-startup')) {
    app.quit();
}

// set-up context menu for dev-env
if (isDev) {
    contextMenu();
}


// create window -------------------------------------------------------------------------------------------------------

const createWindow = (): BrowserWindow => {

    // create the browser window
    const win = new BrowserWindow({
        width:                 1024,
        height:                768,
        minWidth:              1024,
        minHeight:             768,
        darkTheme:             true,
        backgroundColor:       background.chrome,
        show:                  false,
        webPreferences:       {
            nodeIntegration:   true,
        },
    });
    win.setRepresentedFilename('/Users/rakesh/Temp/brand.svg');

    // show window when ready
    win.once('ready-to-show', () => win.show());

    // load index.html
    win.loadURL(MAIN_WINDOW_WEBPACK_ENTRY + '#document');

    // open dev-tools in dev-env & install extensions
    if (isDev) {
        win.webContents.openDevTools();
    }

    return win;
};


// app events ----------------------------------------------------------------------------------------------------------

// install dev-tools & create window when app is ready
app.on('ready', () => {

    // install dev-tools if in dev-env
    if (isDev) {
        installExtension(REACT_DEVELOPER_TOOLS);
    }

    // create main window
    createWindow();
});

// quit when all windows are closed.
app.on('window-all-closed', app.quit);
