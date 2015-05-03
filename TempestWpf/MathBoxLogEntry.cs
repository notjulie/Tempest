using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace TempestWpf
{
   /// <summary>
   /// Represents an entry in the MathBox log
   /// </summary>
   public class MathBoxLogEntry
   {
      private Dictionary<string, string> attributes = new Dictionary<string, string>();

      /// <summary>
      /// Gets the value of a named attribute
      /// </summary>
      /// <param name="name">the name</param>
      /// <returns>the value</returns>
      public string GetAttribute(string name)
      {
         return attributes[name];
      }

      /// <summary>
      /// Constructs an instance from the gicen XML element
      /// </summary>
      /// <param name="element">the XML element</param>
      /// <returns>the resulting instance</returns>
      public static MathBoxLogEntry FromElement(XmlElement element)
      {
         MathBoxLogEntry result = new MathBoxLogEntry();

         // parse all the child elements
         // parse all the entries
         for (int i = 0; i < element.ChildNodes.Count; ++i)
         {
            XmlElement child = element.ChildNodes[i] as XmlElement;
            if (child == null)
               continue;

            switch (child.Name)
            {
               default:
                  result.attributes[child.Name] = child.GetAttribute("value");
                  break;
            }
         }

         return result;
      }
   }
}
