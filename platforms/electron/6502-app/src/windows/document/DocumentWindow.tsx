//
//  DocumentWindow.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React         from 'react';

import IpcListener   from '../../components/utility/IpcListener';


export interface Props {
    filePath:    string;
}

export default class DocumentWindow extends React.PureComponent<Props> {

    // events ----------------------------------------------------------------------------------------------------------

    private _ipc_onSave = (): void => {
        console.log('--debug save');
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return (
            <div className='window welcome'>
                <IpcListener channel='menu.save' listener={this._ipc_onSave}/>
                <h1>Document Window: {this.props.filePath}</h1>
            </div>
        );
    }
}
