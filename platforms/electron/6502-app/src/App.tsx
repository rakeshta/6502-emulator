//
//  App.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React         from 'react';

import TabContainer, {
    Tab,
}                    from './components/layout/TabContainer';
import Editor        from './components/editor/Editor';


interface State {
    activeTab: string;
}

export default class App extends React.PureComponent<{}, State> {

    public state: State = {
        activeTab: 'edit:untitled-1',
    };


    // events ----------------------------------------------------------------------------------------------------------

    private _tabContainer_onSelect = (tab: Tab<string>): void => {
        this.setState({activeTab: tab.key});
    }


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        const {activeTab} = this.state;
        return (
            <TabContainer
                className='mt-2'
                tabs={[
                    {key: 'edit:untitled-1', title: 'Untitled 1', context: 'Untitled 1'},
                    {key: 'edit:untitled-2', title: 'Untitled 2', context: 'Untitled 2'},
                    {key: 'edit:__new__',    title: '+',          context: 'New'},
                ]}
                activeKey={activeTab}
                render={this._renderTab}
                onSelect={this._tabContainer_onSelect}
            />
        );
    }

    private _renderTab = (tab: Tab<string>): React.ReactNode => (
        <h1>Tab Contents: {tab.context}</h1>
    )
}
