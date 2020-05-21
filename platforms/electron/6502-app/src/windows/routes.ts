//
//  routes.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React           from 'react';

import WelcomeWindow   from './welcome/WelcomeWindow';
import DocumentWindow  from './document/DocumentWindow';


export interface Route {
    key:         string;
    component:   React.ComponentType;
}

const routes: Route[] = [
    {key: 'welcome',  component: WelcomeWindow},
    {key: 'document', component: DocumentWindow},
];

export default routes;
