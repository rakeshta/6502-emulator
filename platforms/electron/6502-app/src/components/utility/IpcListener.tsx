//
//  IpcListener.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 23 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React           from 'react';
import {
    ipcRenderer,
    IpcRendererEvent,
}                      from 'electron';


export interface Props {
    channel:   string;
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    listener: (event: IpcRendererEvent, ...args: any[]) => void;
}

export default class IpcListener extends React.PureComponent<Props> {

    // lifecycle -------------------------------------------------------------------------------------------------------

    public componentDidMount(): void {
        ipcRenderer.on(this.props.channel, this._on);
    }

    public componentWillUnmount(): void {
        ipcRenderer.removeListener(this.props.channel, this._on);
    }

    public componentDidUpdate(prevProps: Props): void {

        // channel changing. unregister from old channel & re-register to new one
        if (prevProps.channel !== this.props.channel) {
            ipcRenderer.removeListener(prevProps.channel, this._on);
            ipcRenderer.on(this.props.channel, this._on);
        }
    }


    // events ----------------------------------------------------------------------------------------------------------

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    private _on = (event: IpcRendererEvent, ...args: any[]): void => {
        const {listener} = this.props;
        listener(event, ...args);
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode { return null; }
}
