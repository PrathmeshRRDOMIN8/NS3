/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1 ---------n2
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  std :: string animFile ="somename.xml";
  
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (4);
  NodeContainer n0n2 = NodeContainer (nodes.Get (0), nodes.Get(2));
  NodeContainer n1n2 = NodeContainer (nodes.Get (1), nodes.Get(2));
  NodeContainer n2n3 = NodeContainer (nodes.Get (2), nodes.Get(3));
  
  PointToPointHelper p2p1;
  p2p1.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("2p"));
  p2p1.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p1.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d0d2 = p2p1.Install (n0n2);
  
  PointToPointHelper p2p2;
  p2p2.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("2p"));
  p2p2.SetDeviceAttribute ("DataRate", StringValue ("8Mbps"));
  p2p2.SetChannelAttribute ("Delay", StringValue ("3ms"));
  NetDeviceContainer d1d2 = p2p2.Install (n1n2);
  
  PointToPointHelper p2p3;
  p2p3.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("2p"));
  p2p3.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p3.SetChannelAttribute ("Delay", StringValue ("4ms"));
  NetDeviceContainer d2d3 = p2p3.Install (n2n3);

  InternetStackHelper stack;
  stack.Install (nodes);

  // Later, we add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i2 = ipv4.Assign (d0d2);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);
  
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i2i3 = ipv4.Assign (d2d3);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  //non2
  UdpEchoServerHelper echoServer1 (9);
  ApplicationContainer serverApps1 = echoServer1.Install (nodes.Get (2));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (10.0));
  
  //n1n2
  UdpEchoServerHelper echoServer2 (10);
  ApplicationContainer serverApps2 = echoServer2.Install (nodes.Get (2));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (10.0));
  
  //n2n3
  UdpEchoServerHelper echoServer3 (11);
  ApplicationContainer serverApps3 = echoServer3.Install (nodes.Get (3));
  serverApps3.Start (Seconds (1.0));
  serverApps3.Stop (Seconds (10.0));
  
  
  UdpEchoClientHelper echoClient1 (i0i2.GetAddress (1), 9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (5.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientApps1 = echoClient1.Install (nodes.Get (0));
  clientApps1.Start (Seconds (2.0));
  clientApps1.Stop (Seconds (10.0));
  
 
  UdpEchoClientHelper echoClient2 (i1i2.GetAddress (1), 10);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (5.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientApps2 = echoClient2.Install (nodes.Get (1));
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (10.0));
  

  UdpEchoClientHelper echoClient3 (i2i3.GetAddress (1), 11);
  echoClient3.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient3.SetAttribute ("Interval", TimeValue (Seconds (5.0)));
  echoClient3.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientApps3 = echoClient3.Install (nodes.Get (2));
  clientApps3.Start (Seconds (2.0));
  clientApps3.Stop (Seconds (10.0));

  AnimationInterface anim (animFile);
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
