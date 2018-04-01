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

class ButtonView : RectangleShaderButton {
   private var tempest : cTempest = 0;
   private var whichButton : cTempestButton = FIRE;

   required init?(coder aDecoder: NSCoder) {
     super.init(coder: aDecoder)
   }

   init(view:MTKView, shaderName:String, tempest:cTempest, whichButton:cTempestButton) {
   // call the parent
   super.init(view:view, shaderName:shaderName);
     
   // save parameters
   self.tempest = tempest;
   self.whichButton = whichButton;

   // initialize
   isUserInteractionEnabled = true;
   self.addTarget(self, action: #selector(buttonDown), for: UIControlEvents.touchDown);
   self.addTarget(self, action: #selector(buttonUp), for: UIControlEvents.touchUpInside);
   self.addTarget(self, action: #selector(buttonUp), for: UIControlEvents.touchUpOutside);
   }

   func buttonDown() {
     // tell Tempest that the button is down
     cTempestSetButtonState(tempest, whichButton, 1);
   }

   func buttonUp() {
     // tell Tempest that the button is up
     cTempestSetButtonState(tempest, whichButton, 0);
   }
   
   override func render(renderEncoder : MTLRenderCommandEncoder) {
      if (cTempestIsButtonLit(tempest, whichButton) != 0) {
         super.render(renderEncoder:renderEncoder)
      }
   }

}

