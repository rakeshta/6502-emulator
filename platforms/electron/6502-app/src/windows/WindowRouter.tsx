//
//  WindowRouter.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React            from 'react';

import ThemeController  from '../styles/ThemeController';

import WelcomeWindow    from './welcome/WelcomeWindow';
import DocumentWindow   from './document/DocumentWindow';


// routes --------------------------------------------------------------------------------------------------------------

// eslint-disable-next-line @typescript-eslint/no-explicit-any
function _component(type?: string): React.ComponentType<any> {
    switch (type) {
        case 'welcome':  return WelcomeWindow;
        case 'document': return DocumentWindow;
        default:         return React.memo(function NotFound() {
            return (
                <div className='window not-found'>
                    <h1>Invalid route: {type || '<null>'}</h1>
                </div>
            );
        });
    }
}


// helpers -------------------------------------------------------------------------------------------------------------

function _routeInfo(): {type: string; params: Record<string, string>} | undefined {

    // extract type & query from window URL
    const hash   = window.location.hash;
    if (!hash || hash.length < 2) {
        return undefined;
    }

    const parts = hash.split('?');
    const type  = parts[0].substring(1);
    const query = parts.length > 0 && parts[1];

    // parse query
    const params: Record<string, string> = {};
    if (query && query.length > 0) {
        query.split('&').forEach((item) => {
            const [key, val] = item.split('=');
            params[key] = decodeURIComponent(val);
        });
    }

    return {type, params};
}


// component WindowRouter ----------------------------------------------------------------------------------------------

const WindowRouter = React.memo(() => {

    // extract route info & get component
    const route     = _routeInfo();
    const Component = _component(route?.type);

    // render
    return (
        <React.Fragment>
            <ThemeController/>
            <Component {...route?.params}/>
        </React.Fragment>
    );
});

WindowRouter.displayName = 'WindowRouter';

export default WindowRouter;
