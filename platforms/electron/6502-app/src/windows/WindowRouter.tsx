//
//  WindowRouter.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React      from 'react';

import routes     from './routes';


const WindowRouter = React.memo(() => {

    // find route
    const hash  = window.location.hash;
    const key   = hash.length > 1 && hash.startsWith('#') ? hash.substring(1) : undefined;
    const route = key && routes.find((r) => r.key === key);

    // render route if found
    if (route) {
        const Component = route.component;
        return <Component />;
    }

    // no route
    return (
        <div className='window not-found'>
            <h1>Route not found</h1>
        </div>
    );
});

WindowRouter.displayName = 'WindowRouter';

export default WindowRouter;
