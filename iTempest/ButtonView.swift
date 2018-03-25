//
//  ButtonView.swift
//  iTempest
//
//  Created by Randy Rasmussen on 12/16/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit

class ButtonView : UIView {
    private var tempest : cTempest = 0;
    private var whichButton : cTempestButton = FIRE;
    private var button : UIButton = UIButton(type: UIButtonType.roundedRect);

    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
    }
    
    init(tempest:cTempest, _whichButton:cTempestButton, color:UIColor) {
        // call the parent
        super.init(frame:CGRect(x:0, y:0, width:80, height:80));
        
        // save parameters
        self.tempest = tempest;
        whichButton = _whichButton;

        // initialize
        button.backgroundColor = color;
        isUserInteractionEnabled = true;
        button.frame = CGRect(
            x: 0,
            y: 0,
            width: 80,
            height: 80);
        button.layer.cornerRadius = 40;
        button.addTarget(self, action: #selector(buttonDown), for: UIControlEvents.touchDown);
        button.addTarget(self, action: #selector(buttonUp), for: UIControlEvents.touchUpInside);
        button.addTarget(self, action: #selector(buttonUp), for: UIControlEvents.touchUpOutside);
        self.addSubview(button);
    }
    
    func buttonDown() {
        // tell Tempest that the button is down
        cTempestSetButtonState(tempest, whichButton, 1);
    }
    
    func buttonUp() {
        // tell Tempest that the button is up
        cTempestSetButtonState(tempest, whichButton, 0);
    }
    
}

