using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace TempestWpf
{
   /// <summary>
   /// Represents a log entry for one of the ALUs
   /// </summary>
   public class ALULogEntry
   {
      private Dictionary<string, string> attributes = new Dictionary<string, string>();
      private string name = string.Empty;

      /// <summary>
      /// Gets the name
      /// </summary>
      public string Name
      {
         get
         {
            return name;
         }
      }

      /// <summary>
      /// Gets the value of a named attribute
      /// </summary>
      /// <param name="name">the name</param>
      /// <returns>the value</returns>
      public string GetAttribute(string name)
      {
         if (attributes.ContainsKey(name))
            return attributes[name];
         else
            return string.Empty;
      }

      /// <summary>
      /// Constructs an instance from the given XML element
      /// </summary>
      /// <param name="element">the XML element</param>
      /// <returns>the created instance</returns>
      public static ALULogEntry FromXML(XmlElement element)
      {
         ALULogEntry result = new ALULogEntry();

         // get the name
         result.name = element.GetAttribute("Name");

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
