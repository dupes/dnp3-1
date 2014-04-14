using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Adapter;
using DNP3.Interface;

namespace DERTestSet.dnp3
{
    public class OutstationManager
    {
        IDNP3Manager m_manager;

        public OutstationManager()
        {
            m_manager = DNP3ManagerFactory.CreateManager(Environment.ProcessorCount);
        }
    }
}
