//
//  SpinnerRenderer.swift
//  iTempest
//
//  Created by Randy Rasmussen on 3/10/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

class SpinnerRenderer : MetalRenderer {
   public var rotation : Float = 0;

   private var renderPipelineState: MTLRenderPipelineState?
   private var vertexBuffer: MTLBuffer! = nil;
   private var vertexData : [SpinnerVertex] = []

   override init(view:MTKView) {
      // call the super
      super.init(view: view)
      
      // Render pipeline
      let library = view.device!.newDefaultLibrary()!
      let vertexFunction = library.makeFunction(name: "spinnerVertex")
      let fragmentFunction = library.makeFunction(name: "spinnerFragment")
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
      
      // create our vertex data... this never changes, we just update the rotation
      let bumpCount = 30
      var i : Int = 0;
      while (i<bumpCount)
      {
         var position : Float = Float(i);
         position = position / Float(bumpCount - 1)
         vertexData += [SpinnerVertex(position: position)]
         i = i + 1
      }
      let dataSize = vertexData.count * MemoryLayout.size(ofValue: vertexData[0]);
      vertexBuffer = view.device!.makeBuffer(bytes: vertexData, length: dataSize, options: []);
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
      renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, at: Int(SPINNER_VERTICES_BUFFER));
      renderEncoder.setVertexBuffer(renderParametersBuffer, offset: 0, at: Int(SPINNER_RENDER_PARAMETERS_BUFFER));
      renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6*vertexData.count, instanceCount: 1)
   }
   
}
