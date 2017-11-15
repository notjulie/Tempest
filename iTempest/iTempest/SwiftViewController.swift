//
//  SwiftViewController.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/14/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit


class SwiftViewController : UIViewController
{
    var tempest : cTempest;
    var tempestView : TempestView;
    
    init() {
        // create our Tempest machine
        self.tempest = cTempestCreate();
        self.tempestView = TempestView();

        super.init(nibName: nil, bundle: nil);
    }
    
    required init?(coder aDecoder: NSCoder) {
        // create our Tempest machine
        self.tempest = cTempestCreate();
        self.tempestView = TempestView();

        // call the super
        super.init(coder: aDecoder);
    }
    
    override func viewDidLoad() {
        // call the super
        super.viewDidLoad();
        
        // set our background
        self.view.layer.backgroundColor = UIColor.brown.cgColor;
        
        // initialize the TempestView
        self.tempestView.frame = CGRect(x:0,y:0,width:200,height:200);
        self.tempestView.backgroundColor = UIColor.red;
        self.view.addSubview(tempestView);

        // set up our function that gets called on screen updates
        //CADisplayLink *displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update)];
        //displayLink.preferredFramesPerSecond = 60;
        //[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    }
}
