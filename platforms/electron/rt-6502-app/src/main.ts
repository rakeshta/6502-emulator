//
//  main.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Pixxa LLC. All rights reserved.
//

import { app, BrowserWindow } from 'electron';
import contextMenu            from 'electron-context-menu';
import windowStateKeeper      from 'electron-window-state';
import isDev                  from 'electron-is-dev';


declare const MAIN_WINDOW_WEBPACK_ENTRY: any;


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
        x:         windowState.x,
        y:         windowState.y,
        width:     windowState.width,
        height:    windowState.height,
        minWidth:  1024,
        minHeight: 768,
    });

    // let window state manager listen to window size changes
    windowState.manage(win);

    // load index.html
    win.loadURL(MAIN_WINDOW_WEBPACK_ENTRY);

    // open dev-tools in dev-env
    if (isDev) {
        win.webContents.openDevTools();
    }
});

// Quit when all windows are closed.
app.on('window-all-closed', app.quit);