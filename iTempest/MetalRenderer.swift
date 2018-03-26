//
//  MetalRenderer.swift
//  iTempest
//
//  Created by Randy Rasmussen on 3/11/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

class MetalRenderer {
   public let view : MTKView
   
   public var frame : CGRect = CGRect()
   
   init(view:MTKView) {
      self.view = view
   }
   
   /**
    Abstract rendering function
    */
   func render(renderEncoder : MTLRenderCommandEncoder) {
   }
}
