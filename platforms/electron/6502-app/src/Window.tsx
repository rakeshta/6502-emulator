//
//  Window.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React      from 'react';


export default class Window extends React.PureComponent {

    public render(): React.ReactNode {
        return (
            <div className='m-2'>
                <h1>App Window</h1>
            </div>
        );
    }
}
