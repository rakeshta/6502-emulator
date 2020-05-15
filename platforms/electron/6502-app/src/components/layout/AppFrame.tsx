//
//  AppFrame.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 15 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React   from 'react';
import Nav     from 'react-bootstrap/Nav';


export interface Props {
    className?:  string;
    children?:   React.ReactNode;
}

export default class AppFrame extends React.PureComponent<Props> {

    public render(): React.ReactNode {
        return (
            <React.Fragment>
                <Nav className='top-nav mt-2' variant='tabs' defaultActiveKey='assembly'>
                    <Nav.Item><Nav.Link eventKey='assembly'>Assembly</Nav.Link></Nav.Item>
                    <Nav.Item><Nav.Link eventKey='emulator'>Emulator</Nav.Link></Nav.Item>
                </Nav>
                <main>
                    {this.props.children}
                </main>
            </React.Fragment>
        );
    }
}
