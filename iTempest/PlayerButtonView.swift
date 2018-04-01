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
   
   override func render(renderEncoder : MTLRenderCommandEncoder) {
      // we only display the player buttons during attract mode
      if (cTempestIsInAttractMode(tempest) != 0) {
         if (scale < 1.0) {
            scale = scale + 1.0 / 20
         }
      } else {
         if (scale > 0.0) {
            scale = scale - 1.0 / 20
         }
      }

      // set the frame of the image
      image!.frame = CGRect(
         x: self.frame.midX - self.frame.width * scale / 2,
         y: self.frame.midY - self.frame.height * scale / 2,
         width: self.frame.width * scale,
         height: self.frame.height * scale
      )
      
      // and render if visible
      if (cTempestIsButtonLit(tempest, whichButton) != 0) {
         image!.render(renderEncoder: renderEncoder)
      }
   }
}
