//
//  SpinnerHub.swift
//  iTempest
//
//  Created by Randy Rasmussen on 3/13/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

class SpinnerHub : MetalRenderer {
   private var rotation : Float = 0;
   private var renderPipelineState: MTLRenderPipelineState?

   override init(view:MTKView) {
      // call the super
      super.init(view: view)
      
      // Render pipeline
      let library = view.device!.newDefaultLibrary()!
      let vertexFunction = library.makeFunction(name: "spinnerHubVertex")
      let fragmentFunction = library.makeFunction(name: "spinnerHubFragment")
      let renderPipelineDescriptor = MTLRenderPipelineDescriptor()
      renderPipelineDescriptor.vertexFunction = vertexFunction
      renderPipelineDescriptor.fragmentFunction = fragmentFunction
      renderPipelineDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat
      renderPipelineDescriptor.depthAttachmentPixelFormat = view.depthStencilPixelFormat
      do {
         renderPipelineState = try view.device!.makeRenderPipelineState(descriptor: renderPipelineDescriptor)
      } catch {
         Swift.print("Unable to compile render pipeline state")
         return
      }
   }


   func render(renderEncoder : MTLRenderCommandEncoder) {
      // install our metal functions
      renderEncoder.setRenderPipelineState(renderPipelineState!)
      
      // set our render parameters
      let renderParameters : SpinnerRenderParameters =
         SpinnerRenderParameters(
            rotation: self.rotation,
            centerX: Float(-1 + 2 * self.frame.midX / view.frame.width),
            centerY: Float(1 - 2 * self.frame.midY / view.frame.height),
            xScale: Float(self.frame.width / view.frame.width),
            yScale: Float(self.frame.height / view.frame.height)
      )
      
      let dataSize = MemoryLayout.size(ofValue: renderParameters);
      let renderParametersBuffer: MTLBuffer = renderEncoder.device.makeBuffer(bytes: [renderParameters], length: dataSize, options: []);
      
      // encode our render commands
      renderEncoder.setVertexBuffer(renderParametersBuffer, offset: 0, at: Int(SPINNER_RENDER_PARAMETERS_BUFFER));
      renderEncoder.setFragmentBuffer(renderParametersBuffer, offset: 0, at: Int(SPINNER_RENDER_PARAMETERS_BUFFER));
      renderEncoder.drawPrimitives(type: .triangleStrip, vertexStart: 0, vertexCount: 4, instanceCount: 1)
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

