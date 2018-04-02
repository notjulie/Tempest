//
//  StartButtonView.swift
//  iTempest
//
//  Created by Randy Rasmussen on 4/1/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

/**
 * Child of ButtonView specific to one-player and two-player start buttons
 */
class PlayerButtonView : ButtonView {
   private var scale : CGFloat = 0
   
   required init?(coder aDecoder: NSCoder) {
      super.init(coder: aDecoder)
   }
   
   init(view:MTKView, tempest:cTempest, whichButton:cTempestButton) {
      // call the parent
      super.init(view:view, shaderName:"playerButtonShader", tempest:tempest, whichButton:whichButton);
   }
   
   /**
    * returns the render frame of our button; this is smaller that the view if we
    * are in the middle of upscaling/downscaling
    */
   public override func getRenderFrame() -> CGRect {
      return CGRect(
         x: self.frame.midX - self.frame.width * scale / 2,
         y: self.frame.midY - self.frame.height * scale / 2,
         width: self.frame.width * scale,
         height: self.frame.height * scale
      )
   }

   
   override func render(renderEncoder : MTLRenderCommandEncoder) {
      // we only display the player buttons during attract mode; if we are transitioning
      // to or from attract mode we scale the button in/out
      if (cTempestIsInAttractMode(tempest) != 0) {
         if (scale < 1.0) {
            scale = scale + 1.0 / 20
         }
      } else {
         if (scale > 0.0) {
            scale = scale - 1.0 / 20
         }
      }
      
      // set our render parameters
      let renderParameters =
         PlayerButtonSettings(
            brightness:cTempestIsButtonLit(tempest, whichButton) != 0 ? 1.0 : 0.5
      )
      let renderParametersBuffer: MTLBuffer = renderEncoder.device.makeBuffer(
         bytes: [renderParameters],
         length: MemoryLayout.size(ofValue: renderParameters),
         options: [])
      super.setFragmentBuffer(index:Int(PLAYER_BUTTON_SETTINGS_BUFFER_INDEX), buffer:renderParametersBuffer)

      // let the base class render
      super.render(renderEncoder: renderEncoder)
   }
}
