//
//  MetalRenderer.swift
//  iTempest
//
//  Created by Randy Rasmussen on 3/11/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

class MetalRenderer : AbstractMetalRenderer {
   public let view : MTKView
   
   private var frame : CGRect = CGRect()
   
   init(view:MTKView) {
      self.view = view
   }

   public func getFrame() -> CGRect {
      return frame
   }
   
   /**
    * Sets the frame for this object
    */
   public func setFrame(frame : CGRect) {
      self.frame = frame
   }
   
   /**
    Abstract rendering function
    */
   func render(renderEncoder : MTLRenderCommandEncoder) {
   }
}
