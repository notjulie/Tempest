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
   
   override init(view:MTKView, tempest:cTempest, whichButton:cTempestButton) {
      // call the parent
      super.init(view:view, tempest:tempest, whichButton:whichButton);
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
      
      // update the brightness
      if (cTempestIsButtonLit(tempest, whichButton) != 0) {
         settings.r = 0.95
         settings.g = 0.0
         settings.b = 0.1
      } else {
         settings.r = 0.75
         settings.g = 0.0
         settings.b = 0.2
      }
      
      // call the parent
      super.render(renderEncoder: renderEncoder)
   }
}
