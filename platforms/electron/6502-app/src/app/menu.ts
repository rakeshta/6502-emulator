//
//  menu.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import {
    app,
    dialog,
    shell,
    BrowserWindow,
    Menu,
    MenuItemConstructorOptions,
}                  from 'electron';
import isDev       from 'electron-is-dev';

import controller  from './controller';
import config      from '../config';


// helpers -------------------------------------------------------------------------------------------------------------

const isMac = process.platform === 'darwin';

type MiOptions = MenuItemConstructorOptions;
function _m(m: MiOptions): MiOptions;
function _m(m: MiOptions[]): MiOptions[];
function _m(m: MiOptions | MiOptions[]): MiOptions | MiOptions[] {
    return m;
}

const template: MiOptions[] = [];


// app menu ------------------------------------------------------------------------------------------------------------

if (isMac) {
    template.push({
        label: app.name,
        submenu: [
            { role: 'about' },
            { type: 'separator' },
            { role: 'services' },
            { type: 'separator' },
            { role: 'hide' },
            { role: 'hideOthers' },
            { role: 'unhide' },
            { type: 'separator' },
            { role: 'quit' }
        ],
    });
}


// file menu -----------------------------------------------------------------------------------------------------------

template.push({
    label: 'File',
    submenu: [
        {
            label:       'New',
            accelerator: 'CmdOrCtrl+N',
            click:       (): void => {
                controller.document.new();
            },
        },
        {
            label:       'New From Template...',
            accelerator: 'CmdOrCtrl+Shift+N',
            click:       (): void => console.log('--debug menu: File > New From Template'),
        },
        {
            label:       'Open',
            accelerator: 'CmdOrCtrl+O',
            click:       async (): Promise<void> => {

                // show open dialog
                const res = await dialog.showOpenDialog({
                    filters:     config.file.filters,
                    properties: ['openFile'],
                });
                if (res.canceled) {
                    return;
                }

                // open file in a new window
                controller.document.open(res.filePaths[0]);
            },
        },
        { role: 'recentDocuments' },
        { type: 'separator' },
        {
            label:       'Save',
            accelerator: 'CmdOrCtrl+S',
            click:       (menuItem, browserWindow): void => {
                browserWindow.webContents.send('menu.save');
            },
        },
        {
            label:       'Save As...',
            accelerator: 'CmdOrCtrl+Shift+S',
            click:       (): void => console.log('--debug menu: File > Save As...'),
        },
        {
            label:       'Save All',
            accelerator: 'CmdOrCtrl+Alt+S',
            click:       (): void => {
                BrowserWindow.getAllWindows().forEach((win) => win.webContents.send('menu.save'));
            },
        },
        { type: 'separator' },
        ..._m(isMac ? [
            { role: 'close' }
        ] : [
            { role: 'quit' },
        ]),
    ],
});


// edit menu -----------------------------------------------------------------------------------------------------------

template.push({
    label: 'Edit',
    submenu: [
        { role: 'undo' },
        { role: 'redo' },
        { type: 'separator' },
        { role: 'cut' },
        { role: 'copy' },
        { role: 'paste' },
        ..._m(isMac ? [
            { role: 'delete' },
            { role: 'selectAll' },
            { type: 'separator' },
            {
                label: 'Speech',
                submenu: [
                    { role: 'startSpeaking' },
                    { role: 'stopSpeaking' },
                ]
            }
        ] : [
            { role: 'delete' },
            { type: 'separator' },
            { role: 'selectAll' }
        ]),
    ]
},);


// view menu -----------------------------------------------------------------------------------------------------------

template.push({
    label: 'View',
    submenu: [
        ..._m(isDev ? [
            { role: 'reload' },
            { role: 'forceReload' },
            { role: 'toggleDevTools' },
            { type: 'separator' },
        ] : []),
        { role: 'zoomIn' },
        { role: 'zoomOut' },
        { role: 'resetZoom' },
        { type: 'separator' },
        { role: 'togglefullscreen' }
    ],
});


// window menu ---------------------------------------------------------------------------------------------------------

template.push({
    label: 'Window',
    submenu: [
        { role: 'minimize' },
        { role: 'zoom' },
        { type: 'separator' },
        { label: 'Welcome', click: (): void => {
            controller.welcome.show();
        }},
        ..._m(isMac ? [
            { type: 'separator' },
            { role: 'front' },
            { type: 'separator' },
            { role: 'window' }
        ] : [
            { role: 'close' }
        ])
    ]
});


// help menu -----------------------------------------------------------------------------------------------------------

template.push({
    role: 'help',
    submenu: [
        {
            label: 'Learn More',
            click: (): void => {
                shell.openExternal('https://github.com/rakeshta/6502-emulator/tree/master/platforms/electron/6502-app');
            },
        },
        ..._m(isMac ? [] : [
            { type: 'separator' },
            { role: 'about' },
        ]),
    ],
});



// build & export ------------------------------------------------------------------------------------------------------

export default Menu.buildFromTemplate(template);
