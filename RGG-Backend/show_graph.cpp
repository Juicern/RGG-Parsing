/*****************************************************************************
***
Copyright 2021 by Juicern(JuicernChu@outlook.com).
All rights reserved.

Permission to use, copy, modifyand distribute this software for personaland educational use is hereby granted without fee, provided that the above copyright notice appears in all copiesand that both that copyright noticeand this permission notice appear in supporting documentation, and that the names of Addison Wesley Longman or the author are not used in advertising or publicity pertaining to distribution of the software without specific, written prior permission.Addison Wesley Longmanand the author make no representations about the suitability of this software for any purpose.It is provided "as is" without express or implied warranty.

ADDISON WESLEY LONGMAN AND THE AUTHOR DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.IN NO EVENT SHALL ADDISON WESLEY LONGMAN OR THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
***
******************************************************************************/

#pragma once
#include "show_graph.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace {
std::string escape_json(const std::string& value) {
	std::string escaped;
	escaped.reserve(value.size());
	for (const auto ch : value) {
		switch (ch) {
		case '"':
			escaped += "\\\"";
			break;
		case '\\':
			escaped += "\\\\";
			break;
		case '\n':
			escaped += "\\n";
			break;
		case '\r':
			escaped += "\\r";
			break;
		case '\t':
			escaped += "\\t";
			break;
		default:
			escaped += ch;
			break;
		}
	}
	return escaped;
}

std::string graph_elements_json(const Graph& graph) {
	std::ostringstream oss;
	oss << "{nodes:[";
	for (size_t i = 0; i < graph.nodes.size(); ++i) {
		const auto& node = graph.nodes[i];
		oss << "{data:{id:\"node_" << node.id << "\",label:\""
			<< escape_json(node.label) << "\",terminal:" << (node.is_terminal ? "true" : "false")
			<< "}}";
		if (i + 1 != graph.nodes.size()) {
			oss << ",";
		}
	}
	oss << "],edges:[";
	for (size_t i = 0; i < graph.edges.size(); ++i) {
		const auto& edge = graph.edges[i];
		oss << "{data:{id:\"edge_" << edge.id << "\",source:\"node_" << edge.point1.first.id
			<< "\",target:\"node_" << edge.point2.first.id << "\"}}";
		if (i + 1 != graph.edges.size()) {
			oss << ",";
		}
	}
	oss << "]}";
	return oss.str();
}
}

/// <summary>
/// Open process.html file in the default browser (best-effort cross platform)
/// </summary>
void show_process() {
#if defined(_WIN32)
	system("start process.html");
#elif defined(__APPLE__)
	system("open process.html");
#else
	system("xdg-open process.html");
#endif
}

/// <summary>
/// Draw each graph into process.html using Cytoscape.js
/// </summary>
/// <param name="graphs">ordered graphs in the reduction trace</param>
void draw_process_in_html(const std::vector<Graph>& graphs) {
	std::ofstream file("process.html", std::ios::out);
	if (!file.is_open()) {
		return;
	}
	file << "<!doctype html>\n<html>\n<head>\n<meta charset=\"utf-8\" />\n";
	file << "<title>RGG Parsing Trace</title>\n";
	file << "<style>\nbody{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;background:#f5f5f5;margin:0;padding:1.5rem;}\n";
	file << "h1{margin-top:0;}section{background:#fff;border-radius:8px;padding:1rem;margin-bottom:1rem;box-shadow:0 1px 3px rgba(0,0,0,0.08);}\n";
	file << ".graph-container{width:100%;height:420px;border:1px solid #e1e1e1;border-radius:6px;margin-top:1rem;}\n";
	file << ".controls{display:flex;align-items:center;gap:1rem;margin-top:1rem;}\n";
	file << ".controls button{background:#0f7bff;color:#fff;border:none;border-radius:4px;padding:0.5rem 1.25rem;font-size:0.95rem;font-weight:600;cursor:pointer;}\n";
	file << ".controls button:disabled{opacity:0.5;cursor:not-allowed;}\n";
	file << ".controls span{font-weight:600;}\n";
	file << "</style>\n";
	file << "<script src=\"https://unpkg.com/cytoscape@3.26.0/dist/cytoscape.min.js\"></script>\n";
	file << "</head>\n<body>\n";
	file << "<h1>RGG Parsing Trace</h1>\n";
	file << "<section>\n";
	file << "<div class=\"controls\">\n";
	file << "<button id=\"prev\">⟵ Previous</button>\n";
	file << "<span id=\"status\"></span>\n";
	file << "<button id=\"next\">Next ⟶</button>\n";
	file << "</div>\n";
	file << "<div id=\"graph-view\" class=\"graph-container\"></div>\n";
	file << "</section>\n";
	file << "<script>\n";
	file << "const graphs=[\n";
	for (size_t i = 0; i < graphs.size(); ++i) {
		file << "{label:\"Step " << i << "\",elements:" << graph_elements_json(graphs[i]) << "}";
		if (i + 1 != graphs.size()) {
			file << ",\n";
		}
	}
	file << "\n];\n";
	file << "const container=document.getElementById('graph-view');\n";
	file << "const statusEl=document.getElementById('status');\n";
	file << "const prevBtn=document.getElementById('prev');\n";
	file << "const nextBtn=document.getElementById('next');\n";
	file << "let currentIndex=0;\n";
	file << "const cy=cytoscape({\n";
	file << "  container,\n";
	file << "  elements:[],\n";
	file << "  layout:{name:'breadthfirst',directed:true,padding:25},\n";
	file << "  style:[\n";
	file << "    {selector:'node',style:{'label':'data(label)','text-valign':'center','text-halign':'center','color':'#fff','background-color':ele=>ele.data('terminal')?'#0f7bff':'#ff7b00','width':'60px','height':'60px','font-size':'12px','font-weight':'600'}},\n";
	file << "    {selector:'edge',style:{'width':2,'line-color':'#555','target-arrow-shape':'triangle','target-arrow-color':'#555','curve-style':'bezier'}},\n";
	file << "    {selector:':selected',style:{'border-width':3,'border-color':'#111'}}\n";
	file << "  ],\n";
	file << "  wheelSensitivity:0.15\n";
	file << "});\n";
	file << "function renderGraph(index){\n";
	file << "  if(graphs.length===0){\n";
	file << "    statusEl.textContent='No graphs to display.';\n";
	file << "    prevBtn.disabled=true;\n";
	file << "    nextBtn.disabled=true;\n";
	file << "    return;\n";
	file << "  }\n";
	file << "  currentIndex=(index+graphs.length)%graphs.length;\n";
	file << "  const {label,elements}=graphs[currentIndex];\n";
	file << "  statusEl.textContent=`${label} (${currentIndex+1}/${graphs.length})`;\n";
	file << "  cy.elements().remove();\n";
	file << "  cy.add([...elements.nodes,...elements.edges]);\n";
	file << "  cy.layout({name:'breadthfirst',directed:true,padding:25}).run();\n";
	file << "}\n";
	file << "prevBtn.addEventListener('click',()=>renderGraph(currentIndex-1));\n";
	file << "nextBtn.addEventListener('click',()=>renderGraph(currentIndex+1));\n";
	file << "renderGraph(0);\n";
	file << "</script>\n</body>\n</html>\n";
	file.close();
}
