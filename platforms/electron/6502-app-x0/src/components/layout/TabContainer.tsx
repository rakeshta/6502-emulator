//
//  TabContainer.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 20 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React   from 'react';
import Nav     from 'react-bootstrap/Nav';

import classes from 'classnames';

import './TabContainer.scss';


export interface Tab<Context> {
    key:         string;
    title:       string;
    context?:    Context;
}

export interface Props<Context> {
    className?:  string;

    tabs?:       Tab<Context>[];
    activeKey?:  string;

    render?:    (tab: Tab<Context>) => React.ReactNode;

    onSelect?:  (tab: Tab<Context>) => void;
    onClose?:   (tab: Tab<Context>) => void;
}

export default class TabContainer<Context> extends React.PureComponent<Props<Context>> {

    // helpers ---------------------------------------------------------------------------------------------------------

    private _tabForKey(key: string | undefined): Tab<Context> | undefined {
        if (key === undefined) { return undefined; }
        const {tabs} = this.props;
        return tabs && tabs.find((t) => t.key === key);
    }


    // events ----------------------------------------------------------------------------------------------------------

    private _onSelect = (key: string): void => {
        console.log('--debug select', key);
        const {onSelect} = this.props;
        const selectTab  = this._tabForKey(key);
        if (onSelect && selectTab) {
            onSelect(selectTab);
        }
    }

    private _onClose = (key: string): void => {
        console.log('--debug close', key);
        const {onClose} = this.props;
        const closeTab  = this._tabForKey(key);
        if (onClose && closeTab) {
            onClose(closeTab);
        }
    }

    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        const {className, tabs, activeKey, render} = this.props;
        const activeTab = this._tabForKey(activeKey);
        return (
            <div className={classes('tab-container', className)}>

                {/* tab navigator */}
                <Nav className='tab-nav' variant='tabs' activeKey={activeKey} onSelect={this._onSelect}>
                    {tabs && tabs.map((t) => (
                        <Nav.Item key={t.key} data-key={t.key}>
                            <Nav.Link eventKey={t.key}>{t.title}</Nav.Link>
                        </Nav.Item>
                    ))}
                </Nav>

                {/* tab content */}
                {activeTab && render && (
                    <div className='tab-content'>
                        {render(activeTab)}
                    </div>
                )}
            </div>
        );
    }
}
