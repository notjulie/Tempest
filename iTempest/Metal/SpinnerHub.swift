//
//  SpinnerHub.swift
//  iTempest
//
//  Created by Randy Rasmussen on 3/13/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

class SpinnerHub : RectangleShader {
   private var rotation : Float = 0;

   init(view:MTKView) {
      // call the super
      super.init(view: view, shaderName:"spinnerHubFragment")
   }


   override func render(renderEncoder : MTLRenderCommandEncoder) {
      // set our render parameters
      let frame = getFrame()
      let renderParameters : SpinnerRenderParameters =
         SpinnerRenderParameters(
            rotation: self.rotation,
            centerX: Float(-1 + 2 * frame.midX / view.frame.width),
            centerY: Float(1 - 2 * frame.midY / view.frame.height),
            xScale: Float(frame.width / view.frame.width),
            yScale: Float(frame.height / view.frame.height)
      )
      
      // turn them into metal buffers
      let renderParametersBuffer: MTLBuffer = renderEncoder.device.makeBuffer(
         bytes: [renderParameters],
         length: MemoryLayout.size(ofValue: renderParameters),
         options: [])!

      // give them to the fragment function
      renderEncoder.setFragmentBuffer(renderParametersBuffer, offset: 0, index: Int(SPINNER_RENDER_PARAMETERS_BUFFER))
      
      // let the base class handle it from here
      super.render(renderEncoder: renderEncoder)
   }
   

   public func rotate(rotationAngle:Float) {
      rotation = rotation + rotationAngle
      while (rotation < 0) {
         rotation = rotation + Float(360)
      }
      while (rotation > Float(360)) {
         rotation = rotation - Float(360)
      }
   }
}

