//
//  WelcomeWindow.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React    from 'react';


export default class WelcomeWindow extends React.PureComponent {

    public render(): React.ReactNode {
        return (
            <div className='window welcome'>
                <h1>Welcome Window</h1>
            </div>
        );
    }
}