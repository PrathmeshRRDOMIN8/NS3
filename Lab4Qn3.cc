#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
using namespace ns3;

int main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("BulkSendApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);
  NodeContainer nodes,csma_nodes1, csma_nodes2,p2p_nodes,csma_nodes3;
  nodes.Create (8);


  for (int i = 0; i <= 2; i++)
  {
    csma_nodes1.Add(nodes.Get(i));
  }
  for (int i = 2; i <= 4; i++)
  {
    csma_nodes2.Add(nodes.Get(i));
  }

  p2p_nodes.Add(nodes.Get(4));
  p2p_nodes.Add(nodes.Get(5));

  for (int i = 5; i <= 7; i++)
  {
    csma_nodes3.Add(nodes.Get(i));
  }
  CsmaHelper csma, csma2,csma3;
  NetDeviceContainer devicescsma1, devicescsma2, devicescsma3, devicesp2p;
  csma.SetChannelAttribute ("DataRate", StringValue ("5Mbps"));
  csma.SetChannelAttribute ("Delay", StringValue ("2ms"));

  devicescsma1 = csma.Install(csma_nodes1);

  csma2.SetChannelAttribute ("DataRate", StringValue ("5Mbps"));
  csma2.SetChannelAttribute ("Delay", StringValue ("2ms"));

  devicescsma2 = csma2.Install(csma_nodes2);

  csma3.SetChannelAttribute ("DataRate", StringValue ("5Mbps"));
  csma3.SetChannelAttribute ("Delay", StringValue ("2ms"));

  devicescsma3 = csma3.Install(csma_nodes3);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  devicesp2p = p2p.Install(p2p_nodes);

  InternetStackHelper inet;
  inet.Install(nodes);

    Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces1 = address.Assign (devicescsma1);

    Ipv4AddressHelper address2;
  address.SetBase ("10.2.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces2 = address.Assign (devicescsma2);

    Ipv4AddressHelper address3;
  address.SetBase ("10.3.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces3 = address.Assign (devicesp2p);

  Ipv4AddressHelper address4;
  address.SetBase ("10.4.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces4 = address.Assign (devicescsma3);


  Ipv4Address multicastGroup ("225.1.2.4");
  Ipv4StaticRoutingHelper multicast;

  Ptr<Node> multicastRouter = csma_nodes2.Get (0); 
  Ptr<NetDevice> inputIf = devicescsma1.Get(2);  
  NetDeviceContainer outputDevices;  
  outputDevices.Add (devicescsma2.Get(0));  


multicast.AddMulticastRoute (multicastRouter, interfaces2.GetAddress(0),
                            multicastGroup, inputIf, outputDevices);

  Ptr<Node> multicastRouter2 = p2p_nodes.Get (0); 
  Ptr<NetDevice> inputIf2 = devicescsma2.Get(2);  
  NetDeviceContainer outputDevices2;  
  outputDevices2.Add (devicesp2p.Get(0));  
multicast.AddMulticastRoute (multicastRouter2, interfaces2.GetAddress(0),
                            multicastGroup, inputIf2, outputDevices2);
  Ptr<Node> sender = csma_nodes1.Get (0);
  Ptr<NetDevice> senderIf = devicescsma1.Get (0);
  multicast.SetDefaultMulticastRoute (sender, senderIf);

    UdpEchoServerHelper echoServer (9);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  ApplicationContainer serverApps = echoServer.Install (nodes.Get(5));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (multicastGroup, 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (csma_nodes1.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
      Simulator::Run ();
  

  Simulator::Destroy ();
}



