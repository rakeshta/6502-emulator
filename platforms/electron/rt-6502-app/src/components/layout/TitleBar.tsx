//
//  TitleBar.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 12 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React       from 'react';
import classnames  from 'classnames';

import './TitleBar.scss';


export interface Props {
    className?:  string;
    title?:      string;
}

const TitleBar = React.memo((props: Props) => (
    <div className={classnames('title-bar', props.className)}>
        {props.title && <span className='title'>{props.title}</span>}
    </div>
));

TitleBar.displayName = 'TitleBar';

export default TitleBar;
