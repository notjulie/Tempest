//
//  ButtonView.swift
//  iTempest
//
//  Created by Randy Rasmussen on 12/16/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit
import MetalKit

/**
 * Button class that handles up/down events and passes them off to a given
 * button on the Tempest control panel.
 */
class ButtonView : RectangleShaderButton {
   // public read-only properties
   public let tempest : cTempest
   public let whichButton : cTempestButton

   required init?(coder aDecoder: NSCoder) {
      tempest = 0
      whichButton  = FIRE
      super.init(coder: aDecoder)
   }

   init(view:MTKView, shaderName:String, tempest:cTempest, whichButton:cTempestButton) {
      // save parameters
      self.tempest = tempest
      self.whichButton = whichButton
      
      // call the parent
      super.init(view:view, shaderName:shaderName);
      // initialize
      isUserInteractionEnabled = true
      self.addTarget(self, action: #selector(buttonDown), for: UIControlEvents.touchDown)
      self.addTarget(self, action: #selector(buttonUp), for: UIControlEvents.touchUpInside)
      self.addTarget(self, action: #selector(buttonUp), for: UIControlEvents.touchUpOutside)
   }

   func buttonDown() {
     // tell Tempest that the button is down
     cTempestSetButtonState(tempest, whichButton, 1)
   }

   func buttonUp() {
     // tell Tempest that the button is up
     cTempestSetButtonState(tempest, whichButton, 0);
   }
}

