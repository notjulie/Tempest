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
   public var settings = PlayerButtonSettings(r: 0.1, g:0.4, b:1.0)

   required init?(coder aDecoder: NSCoder) {
      tempest = 0
      whichButton  = FIRE
      super.init(coder: aDecoder)
   }

   init(view:MTKView, tempest:cTempest, whichButton:cTempestButton) {
      // save parameters
      self.tempest = tempest
      self.whichButton = whichButton

      // call the parent
      super.init(view:view, shaderName:"playerButtonShader");
      
      // initialize
      isUserInteractionEnabled = true
      self.addTarget(self, action: #selector(buttonDown), for: UIControlEvents.touchDown)
      self.addTarget(self, action: #selector(buttonUp), for: UIControlEvents.touchUpInside)
      self.addTarget(self, action: #selector(buttonUp), for: UIControlEvents.touchUpOutside)
   }

   override func render(renderEncoder : MTLRenderCommandEncoder) {
      // set our render parameters
      let renderParametersBuffer: MTLBuffer = renderEncoder.device.makeBuffer(
         bytes: [settings],
         length: MemoryLayout.size(ofValue: settings),
         options: [])
      super.setFragmentBuffer(index:Int(PLAYER_BUTTON_SETTINGS_BUFFER_INDEX), buffer:renderParametersBuffer)
      
      // let the base class render
      super.render(renderEncoder: renderEncoder)
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

