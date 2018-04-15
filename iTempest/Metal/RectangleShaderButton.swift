//
//  RectangleShaderButton.swift
//  iTempest
//
//  Created by Randy Rasmussen on 3/31/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit
import UIKit

class RectangleShaderButton : UIButton, AbstractMetalRenderer {
   // public read-only properties
   private let image : RectangleShader?;
   
   init(view:MTKView, shaderName:String) {
      // initialize the image
      image = RectangleShader(view:view, shaderName:shaderName)

      // call the parent
      super.init(frame: CGRect(x:0, y:0, width:50, height:50));
   }
   
   required init?(coder aDecoder: NSCoder) {
      image = nil;
      super.init(coder:aDecoder)
   }
   
   /**
    * Returns the frame into which the shader will render; a child class can override this
    * if the rendered rectangle is to be smaller than the view
    */
   public func getRenderFrame() -> CGRect {
      return self.frame
   }
   
   func render(renderEncoder : MTLRenderCommandEncoder) {
      // set the frame of the image
      image!.setFrame(frame:getRenderFrame())
      
      // and render
      image!.render(renderEncoder: renderEncoder)
   }
   
   public func setFragmentBuffer(index:Int, buffer:MTLBuffer) {
      image!.setFragmentBuffer(index: index, buffer: buffer)
   }
}
