// EV Trip Planner - Modern Interactive Dashboard Controller
// Directly communicates with the native C backend engine (ev_server.exe)

const API_BASE = (window.location.protocol === "file:") ? "http://127.0.0.1:8080" : "";

const initialDehradunData = {
  success: true,
  node_count: 10,
  nodes: [
    { id: 0, name: "Clock Tower", edge_count: 5, has_station: 0, charger_count: 0, installation_cost: 0, x: 400, y: 320 },
    { id: 1, name: "ISBT Dehradun", edge_count: 3, has_station: 0, charger_count: 0, installation_cost: 0, x: 210, y: 500 },
    { id: 2, name: "Railway Station", edge_count: 3, has_station: 0, charger_count: 0, installation_cost: 0, x: 340, y: 410 },
    { id: 3, name: "Forest Research Institute", edge_count: 2, has_station: 0, charger_count: 0, installation_cost: 0, x: 190, y: 280 },
    { id: 4, name: "Pacific Mall", edge_count: 4, has_station: 0, charger_count: 0, installation_cost: 0, x: 550, y: 210 },
    { id: 5, name: "Prem Nagar", edge_count: 3, has_station: 0, charger_count: 0, installation_cost: 0, x: 80, y: 210 },
    { id: 6, name: "DIT University", edge_count: 3, has_station: 0, charger_count: 0, installation_cost: 0, x: 620, y: 80 },
    { id: 7, name: "Max Hospital", edge_count: 2, has_station: 0, charger_count: 0, installation_cost: 0, x: 530, y: 130 },
    { id: 8, name: "Survey Chowk", edge_count: 4, has_station: 0, charger_count: 0, installation_cost: 0, x: 470, y: 360 },
    { id: 9, name: "Sahastradhara Road", edge_count: 3, has_station: 0, charger_count: 0, installation_cost: 0, x: 610, y: 290 }
  ],
  edges: [
    { source: 0, source_name: "Clock Tower", destination: 1, dest_name: "ISBT Dehradun", distance: 5.5, time: 0.20 },
    { source: 0, source_name: "Clock Tower", destination: 2, dest_name: "Railway Station", distance: 2.0, time: 0.10 },
    { source: 0, source_name: "Clock Tower", destination: 3, dest_name: "Forest Research Institute", distance: 4.5, time: 0.18 },
    { source: 0, source_name: "Clock Tower", destination: 4, dest_name: "Pacific Mall", distance: 6.5, time: 0.22 },
    { source: 0, source_name: "Clock Tower", destination: 8, dest_name: "Survey Chowk", distance: 1.8, time: 0.10 },
    { source: 1, source_name: "ISBT Dehradun", destination: 2, dest_name: "Railway Station", distance: 4.0, time: 0.15 },
    { source: 1, source_name: "ISBT Dehradun", destination: 5, dest_name: "Prem Nagar", distance: 8.5, time: 0.30 },
    { source: 2, source_name: "Railway Station", destination: 8, dest_name: "Survey Chowk", distance: 2.5, time: 0.12 },
    { source: 3, source_name: "Forest Research Institute", destination: 5, dest_name: "Prem Nagar", distance: 6.0, time: 0.22 },
    { source: 4, source_name: "Pacific Mall", destination: 7, dest_name: "Max Hospital", distance: 4.0, time: 0.15 },
    { source: 4, source_name: "Pacific Mall", destination: 9, dest_name: "Sahastradhara Road", distance: 6.5, time: 0.25 },
    { source: 4, source_name: "Pacific Mall", destination: 6, dest_name: "DIT University", distance: 12.0, time: 0.35 },
    { source: 5, source_name: "Prem Nagar", destination: 6, dest_name: "DIT University", distance: 18.0, time: 0.45 },
    { source: 6, source_name: "DIT University", destination: 9, dest_name: "Sahastradhara Road", distance: 14.0, time: 0.40 },
    { source: 7, source_name: "Max Hospital", destination: 8, dest_name: "Survey Chowk", distance: 4.0, time: 0.16 },
    { source: 9, source_name: "Sahastradhara Road", destination: 8, dest_name: "Survey Chowk", distance: 8.0, time: 0.25 }
  ]
};

let networkData = JSON.parse(JSON.stringify(initialDehradunData));
let currentRoute = null;
let canvas, ctx;
let camera = { x: 50, y: 30, zoom: 0.95 };
let isDragging = false;
let dragStart = { x: 0, y: 0 };
let hoveredNode = null;
let pulseOffset = 0;
let animFrameId = null;

document.addEventListener("DOMContentLoaded", () => {
  setupNavigation();
  setupCanvas();
  renderInitialData();
  setupEventListeners();
  fetchNetworkData();
  startCanvasAnimation();
});

// Render immediate initial data to prevent blank screen or layout jumps
function renderInitialData() {
  updateOverviewStats();
  populateLocationDropdowns();
  populateDemandTable();
  updateInstalledStationsTable();
  drawNetwork();
}

// Sidebar Navigation Tabs
function setupNavigation() {
  const navBtns = document.querySelectorAll(".nav-item button");
  navBtns.forEach((btn) => {
    btn.addEventListener("click", () => {
      const targetPage = btn.getAttribute("data-page");
      switchPage(targetPage);
    });
  });
}

function switchPage(pageId) {
  document.querySelectorAll(".nav-item").forEach((el) => el.classList.remove("active"));
  document.querySelectorAll(".page-container").forEach((el) => el.classList.remove("active"));

  const targetBtn = document.querySelector(`.nav-item button[data-page="${pageId}"]`);
  if (targetBtn) {
    targetBtn.parentElement.classList.add("active");
  }

  const targetContainer = document.getElementById(pageId);
  if (targetContainer) {
    targetContainer.classList.add("active");
  }

  if (pageId === "page-network") {
    setTimeout(() => {
      resizeCanvas();
      centerCamera();
      drawNetwork();
    }, 50);
  }
}

// Fetch live graph from C Backend Server
async function fetchNetworkData() {
  try {
    const res = await fetch(`${API_BASE}/api/network`);
    if (!res.ok) throw new Error("Status: " + res.status);
    const liveData = await res.json();
    if (liveData && liveData.nodes && liveData.nodes.length > 0) {
      networkData = liveData;
      updateOverviewStats();
      populateLocationDropdowns();
      populateDemandTable();
      updateInstalledStationsTable();
      drawNetwork();
      setBackendStatus(true);
    }
  } catch (err) {
    console.warn("Backend not yet responding on /api/network; using default Dehradun topology:", err);
    setBackendStatus(false);
  }
}

function setBackendStatus(online) {
  const textEl = document.getElementById("backend-status-text");
  const dot = document.querySelector(".status-dot");
  if (textEl) {
    textEl.textContent = online ? "C Engine Online (Port 8080)" : "Local Engine (Starting...)";
  }
  if (dot) {
    dot.style.background = online ? "var(--accent-emerald)" : "var(--accent-amber)";
    dot.style.boxShadow = online ? "0 0 8px var(--accent-emerald)" : "0 0 8px var(--accent-amber)";
  }
}

// Update KPI Stats
function updateOverviewStats() {
  if (!networkData) return;

  const nodeCount = networkData.node_count || (networkData.nodes ? networkData.nodes.length : 10);
  const edgeCount = (networkData.edges || []).length;
  let stationCount = 0;
  let totalChargers = 0;

  (networkData.nodes || []).forEach((n) => {
    if (n.has_station) {
      stationCount++;
      totalChargers += n.charger_count;
    }
  });

  const statNodes = document.getElementById("stat-total-nodes");
  const statEdges = document.getElementById("stat-total-edges");
  const statStations = document.getElementById("stat-total-stations");
  const statChargers = document.getElementById("stat-total-chargers");
  const headerNodes = document.getElementById("hdr-nodes");
  const headerEdges = document.getElementById("hdr-edges");

  if (statNodes) statNodes.textContent = nodeCount;
  if (statEdges) statEdges.textContent = edgeCount;
  if (statStations) statStations.textContent = stationCount;
  if (statChargers) statChargers.textContent = totalChargers;
  if (headerNodes) headerNodes.textContent = nodeCount;
  if (headerEdges) headerEdges.textContent = edgeCount;
}

// Populate Dropdown Selectors
function populateLocationDropdowns() {
  if (!networkData || !networkData.nodes) return;

  const srcSelect = document.getElementById("route-source");
  const dstSelect = document.getElementById("route-dest");
  const installSelect = document.getElementById("install-loc-id");

  const prevSrc = srcSelect ? srcSelect.value : 0;
  const prevDst = dstSelect ? dstSelect.value : 6;

  const optionsHtml = networkData.nodes
    .map((n) => `<option value="${n.id}">${n.name} (ID: ${n.id}${n.has_station ? " ⚡" : ""})</option>`)
    .join("");

  if (srcSelect) {
    srcSelect.innerHTML = optionsHtml;
    srcSelect.value = prevSrc !== undefined && prevSrc < networkData.nodes.length ? prevSrc : 0;
  }
  if (dstSelect) {
    dstSelect.innerHTML = optionsHtml;
    dstSelect.value = prevDst !== undefined && prevDst < networkData.nodes.length ? prevDst : Math.min(6, networkData.nodes.length - 1);
  }
  if (installSelect) {
    installSelect.innerHTML = optionsHtml;
  }
}

// Canvas & Interaction Setup
function setupCanvas() {
  canvas = document.getElementById("networkCanvas");
  if (!canvas) return;
  ctx = canvas.getContext("2d");

  window.addEventListener("resize", () => {
    resizeCanvas();
    drawNetwork();
  });

  resizeCanvas();
  centerCamera();

  canvas.addEventListener("mousedown", (e) => {
    if (e.button === 0) {
      isDragging = true;
      dragStart = { x: e.clientX - camera.x, y: e.clientY - camera.y };
    }
  });

  window.addEventListener("mousemove", (e) => {
    if (isDragging) {
      camera.x = e.clientX - dragStart.x;
      camera.y = e.clientY - dragStart.y;
      drawNetwork();
    } else {
      handleCanvasHover(e);
    }
  });

  window.addEventListener("mouseup", () => {
    isDragging = false;
  });

  canvas.addEventListener("wheel", (e) => {
    e.preventDefault();
    const zoomFactor = e.deltaY < 0 ? 1.1 : 0.9;
    const newZoom = Math.min(Math.max(camera.zoom * zoomFactor, 0.4), 2.5);

    const rect = canvas.getBoundingClientRect();
    const mouseX = e.clientX - rect.left;
    const mouseY = e.clientY - rect.top;

    camera.x = mouseX - (mouseX - camera.x) * (newZoom / camera.zoom);
    camera.y = mouseY - (mouseY - camera.y) * (newZoom / camera.zoom);
    camera.zoom = newZoom;
    drawNetwork();
  });

  canvas.addEventListener("click", (e) => {
    const rect = canvas.getBoundingClientRect();
    const pos = toWorldCoord(e.clientX - rect.left, e.clientY - rect.top);

    if (networkData && networkData.nodes) {
      const clicked = networkData.nodes.find((n) => {
        const dx = n.x - pos.x;
        const dy = n.y - pos.y;
        return Math.sqrt(dx * dx + dy * dy) <= 24;
      });

      if (clicked) {
        handleNodeClick(clicked.id);
      }
    }
  });

  document.getElementById("btn-zoom-in")?.addEventListener("click", () => {
    camera.zoom = Math.min(camera.zoom * 1.2, 2.5);
    drawNetwork();
  });
  document.getElementById("btn-zoom-out")?.addEventListener("click", () => {
    camera.zoom = Math.max(camera.zoom / 1.2, 0.4);
    drawNetwork();
  });
  document.getElementById("btn-reset-zoom")?.addEventListener("click", () => {
    centerCamera();
    drawNetwork();
  });
}

function resizeCanvas() {
  if (!canvas) return;
  const container = canvas.parentElement;
  if (!container) return;

  const w = container.clientWidth || 700;
  const h = container.clientHeight || 550;

  const dpr = window.devicePixelRatio || 1;
  canvas.width = w * dpr;
  canvas.height = h * dpr;
  canvas.style.width = w + "px";
  canvas.style.height = h + "px";

  if (ctx) {
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
  }
}

function centerCamera() {
  if (!canvas) return;
  const w = canvas.clientWidth || 700;
  const h = canvas.clientHeight || 550;
  camera.x = w / 2 - 360 * camera.zoom;
  camera.y = h / 2 - 280 * camera.zoom;
  camera.zoom = 0.95;
}

function toWorldCoord(screenX, screenY) {
  return {
    x: (screenX - camera.x) / camera.zoom,
    y: (screenY - camera.y) / camera.zoom,
  };
}

function handleCanvasHover(e) {
  if (!canvas || !networkData || !networkData.nodes) return;
  const rect = canvas.getBoundingClientRect();
  const pos = toWorldCoord(e.clientX - rect.left, e.clientY - rect.top);

  const prevHovered = hoveredNode;
  hoveredNode = networkData.nodes.find((n) => {
    const dx = n.x - pos.x;
    const dy = n.y - pos.y;
    return Math.sqrt(dx * dx + dy * dy) <= 24;
  });

  if (prevHovered !== hoveredNode) {
    canvas.style.cursor = hoveredNode ? "pointer" : "grab";
    drawNetwork();
  }
}

function handleNodeClick(nodeId) {
  const srcSelect = document.getElementById("route-source");
  const dstSelect = document.getElementById("route-dest");
  if (!srcSelect || !dstSelect) return;

  if (srcSelect.value == nodeId) {
    showNotification(`Node ${nodeId} is already the Source`, "info");
  } else {
    dstSelect.value = nodeId;
    showNotification(`Destination set to: ${networkData.nodes[nodeId].name}`, "success");
    drawNetwork();
  }
}

function startCanvasAnimation() {
  function loop() {
    pulseOffset = (pulseOffset + 0.04) % 1;
    if (currentRoute && currentRoute.path && currentRoute.path.length > 1) {
      drawNetwork();
    }
    animFrameId = requestAnimationFrame(loop);
  }
  loop();
}

// Master Canvas Drawing Function
function drawNetwork() {
  if (!ctx || !networkData || !networkData.nodes) return;

  const dpr = window.devicePixelRatio || 1;
  const w = canvas.width / dpr;
  const h = canvas.height / dpr;

  ctx.clearRect(0, 0, w, h);

  ctx.save();
  ctx.translate(camera.x, camera.y);
  ctx.scale(camera.zoom, camera.zoom);

  // Background Grid
  drawGrid();

  const isEdgeInRoute = (u, v) => {
    if (!currentRoute || !currentRoute.path) return false;
    const p = currentRoute.path;
    for (let i = 0; i < p.length - 1; i++) {
      if ((p[i] === u && p[i + 1] === v) || (p[i] === v && p[i + 1] === u)) {
        return true;
      }
    }
    return false;
  };

  // Draw Edges (Roads)
  (networkData.edges || []).forEach((edge) => {
    const u = networkData.nodes[edge.source];
    const v = networkData.nodes[edge.destination];
    if (!u || !v) return;

    const inRoute = isEdgeInRoute(edge.source, edge.destination);

    ctx.beginPath();
    ctx.moveTo(u.x, u.y);
    ctx.lineTo(v.x, v.y);

    if (inRoute) {
      // Route Highlight Glow
      ctx.strokeStyle = "rgba(0, 240, 255, 0.9)";
      ctx.lineWidth = 5;
      ctx.shadowColor = "#00f0ff";
      ctx.shadowBlur = 14;
      ctx.stroke();

      // Animated electric pulse line
      ctx.beginPath();
      ctx.moveTo(u.x, u.y);
      ctx.lineTo(v.x, v.y);
      ctx.strokeStyle = "#ffffff";
      ctx.lineWidth = 2.5;
      ctx.setLineDash([8, 12]);
      ctx.lineDashOffset = -pulseOffset * 20;
      ctx.stroke();
      ctx.setLineDash([]);
      ctx.shadowBlur = 0;
    } else {
      ctx.strokeStyle = "rgba(56, 189, 248, 0.22)";
      ctx.lineWidth = 2;
      ctx.shadowBlur = 0;
      ctx.stroke();
    }

    // Road Tag Badge
    const midX = (u.x + v.x) / 2;
    const midY = (u.y + v.y) / 2;
    const label = `${edge.distance.toFixed(1)} km`;

    ctx.font = inRoute ? "bold 11px Inter, sans-serif" : "10px Inter, sans-serif";
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";

    const tw = ctx.measureText(label).width;
    ctx.fillStyle = "rgba(7, 11, 20, 0.9)";
    ctx.fillRect(midX - tw / 2 - 4, midY - 7, tw + 8, 14);

    ctx.fillStyle = inRoute ? "#00f0ff" : "#94a3b8";
    ctx.fillText(label, midX, midY);
  });

  // Draw Nodes (Locations)
  const srcVal = parseInt(document.getElementById("route-source")?.value || "-1");
  const dstVal = parseInt(document.getElementById("route-dest")?.value || "-1");

  networkData.nodes.forEach((node) => {
    const isSource = node.id === srcVal;
    const isDest = node.id === dstVal;
    const inRoute = currentRoute && currentRoute.path && currentRoute.path.includes(node.id);
    const isHovered = hoveredNode && hoveredNode.id === node.id;

    // Outer glow halo
    if (isSource || isDest || inRoute || isHovered) {
      ctx.beginPath();
      ctx.arc(node.x, node.y, 24, 0, Math.PI * 2);
      if (isSource) {
        ctx.fillStyle = "rgba(16, 185, 129, 0.25)";
        ctx.strokeStyle = "#10b981";
      } else if (isDest) {
        ctx.fillStyle = "rgba(244, 63, 94, 0.25)";
        ctx.strokeStyle = "#f43f5e";
      } else {
        ctx.fillStyle = "rgba(0, 240, 255, 0.22)";
        ctx.strokeStyle = "#00f0ff";
      }
      ctx.lineWidth = 2;
      ctx.shadowColor = ctx.strokeStyle;
      ctx.shadowBlur = 12;
      ctx.fill();
      ctx.stroke();
      ctx.shadowBlur = 0;
    }

    // Inner node circle
    ctx.beginPath();
    ctx.arc(node.x, node.y, 16, 0, Math.PI * 2);
    if (isSource) {
      ctx.fillStyle = "#10b981";
    } else if (isDest) {
      ctx.fillStyle = "#f43f5e";
    } else if (node.has_station) {
      ctx.fillStyle = "#0070f3";
    } else {
      ctx.fillStyle = "#1e293b";
    }
    ctx.fill();

    ctx.lineWidth = 2;
    ctx.strokeStyle = node.has_station ? "#00f0ff" : "rgba(255, 255, 255, 0.35)";
    ctx.stroke();

    // Node ID text
    ctx.fillStyle = "#ffffff";
    ctx.font = "bold 11px Inter, sans-serif";
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";
    ctx.fillText(node.id.toString(), node.x, node.y);

    // EV Charger Badge
    if (node.has_station) {
      ctx.beginPath();
      ctx.arc(node.x + 12, node.y - 12, 8, 0, Math.PI * 2);
      ctx.fillStyle = "#00f0ff";
      ctx.shadowColor = "#00f0ff";
      ctx.shadowBlur = 6;
      ctx.fill();
      ctx.shadowBlur = 0;

      ctx.fillStyle = "#070b14";
      ctx.font = "bold 9px Inter, sans-serif";
      ctx.fillText("⚡", node.x + 12, node.y - 12);
    }

    // Landmark Name Label
    ctx.font = "12px Inter, sans-serif";
    ctx.textAlign = "center";
    ctx.textBaseline = "top";

    const nameWidth = ctx.measureText(node.name).width;
    ctx.fillStyle = "rgba(7, 11, 20, 0.9)";
    ctx.fillRect(node.x - nameWidth / 2 - 6, node.y + 20, nameWidth + 12, 18);

    ctx.fillStyle = isSource ? "#34d399" : isDest ? "#fb7185" : inRoute ? "#00f0ff" : "#f8fafc";
    ctx.fillText(node.name, node.x, node.y + 22);
  });

  ctx.restore();
}

function drawGrid() {
  const step = 50;
  const w = 1200;
  const h = 800;

  ctx.strokeStyle = "rgba(255, 255, 255, 0.03)";
  ctx.lineWidth = 1;

  for (let x = -w; x <= w * 2; x += step) {
    ctx.beginPath();
    ctx.moveTo(x, -h);
    ctx.lineTo(x, h * 2);
    ctx.stroke();
  }

  for (let y = -h; y <= h * 2; y += step) {
    ctx.beginPath();
    ctx.moveTo(-w, y);
    ctx.lineTo(w * 2, y);
    ctx.stroke();
  }
}

// Event Listeners for All Features
function setupEventListeners() {
  // Calculate Route Button
  document.getElementById("btn-calc-route")?.addEventListener("click", calculateRoute);

  // Swap Route Source and Destination
  document.getElementById("btn-swap-route")?.addEventListener("click", () => {
    const s = document.getElementById("route-source");
    const d = document.getElementById("route-dest");
    if (s && d) {
      const tmp = s.value;
      s.value = d.value;
      d.value = tmp;
      drawNetwork();
      calculateRoute();
    }
  });

  // Source/Dest Change
  document.getElementById("route-source")?.addEventListener("change", () => drawNetwork());
  document.getElementById("route-dest")?.addEventListener("change", () => drawNetwork());

  // Manual Station Installation Form
  document.getElementById("form-install-station")?.addEventListener("submit", async (e) => {
    e.preventDefault();
    const locId = parseInt(document.getElementById("install-loc-id").value);
    const chargers = parseInt(document.getElementById("install-chargers").value);
    const cost = parseFloat(document.getElementById("install-cost").value);

    try {
      const res = await fetch(`${API_BASE}/api/install_station`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          location_id: locId,
          charger_count: chargers,
          installation_cost: cost,
        }),
      });
      const data = await res.json();
      if (data.success) {
        showNotification(data.message, "success");
        await fetchNetworkData();
      } else {
        showNotification(data.error || "Installation failed", "error");
      }
    } catch (err) {
      showNotification("Failed to connect to backend", "error");
    }
  });

  // Auto Station Placement Form (Greedy)
  document.getElementById("form-auto-station")?.addEventListener("submit", async (e) => {
    e.preventDefault();
    const budget = parseFloat(document.getElementById("auto-budget").value);
    const cost = parseFloat(document.getElementById("auto-cost").value);
    const chargers = parseInt(document.getElementById("auto-chargers").value);

    try {
      const res = await fetch(`${API_BASE}/api/auto_station`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ budget, cost, chargers }),
      });
      const data = await res.json();
      if (data.success) {
        showNotification(`Greedy Placement completed: ${data.placed_count} stations placed! Total cost: ₹${data.total_spent}`, "success");
        await fetchNetworkData();
      } else {
        showNotification(data.error || "Placement failed", "error");
      }
    } catch (err) {
      showNotification("Error during automatic placement", "error");
    }
  });

  // Strategy Comparison Form
  document.getElementById("btn-compare-strategies")?.addEventListener("click", runStrategyComparison);

  // Sorting & Searching Actions
  document.getElementById("btn-run-sort-search")?.addEventListener("click", runSortingAndSearch);

  // Stress Benchmark Button
  document.getElementById("btn-run-stress-test")?.addEventListener("click", runLiveStressBenchmark);

  // Reset Network Button
  document.getElementById("btn-reset-network")?.addEventListener("click", async () => {
    if (confirm("Reset network back to the default Dehradun landmarks and roads?")) {
      try {
        const res = await fetch(`${API_BASE}/api/reset_network`, { method: "POST" });
        const d = await res.json();
        currentRoute = null;
        document.getElementById("route-results-box")?.classList.add("hidden");
        showNotification(d.message || "Network reset successfully!", "success");
        await fetchNetworkData();
      } catch (err) {
        // Fallback to local reset
        networkData = JSON.parse(JSON.stringify(initialDehradunData));
        currentRoute = null;
        document.getElementById("route-results-box")?.classList.add("hidden");
        renderInitialData();
        showNotification("Dehradun network reset successfully!", "success");
      }
    }
  });
}

// Calculate Route (Dijkstra, Fastest, Bellman-Ford)
async function calculateRoute() {
  const src = parseInt(document.getElementById("route-source").value);
  const dst = parseInt(document.getElementById("route-dest").value);
  const algo = document.getElementById("route-algo").value;

  if (src === dst) {
    showNotification("Source and Destination cannot be the same location.", "warning");
    return;
  }

  try {
    const res = await fetch(`${API_BASE}/api/route`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ source: src, destination: dst, algorithm: algo }),
    });

    const data = await res.json();
    if (!data.success) {
      showNotification(data.error || "Could not find route", "error");
      currentRoute = null;
      drawNetwork();
      return;
    }

    currentRoute = data;
    renderRouteResults(data);
    drawNetwork();
    showNotification(`Route calculated using ${algo.toUpperCase()}: ${data.total_distance.toFixed(2)} km in ${data.total_time.toFixed(2)} hrs`, "success");
  } catch (err) {
    showNotification("Failed to execute route algorithm on C backend. Is ev_server.exe running?", "error");
  }
}

// Display Route Details in Results Panel
function renderRouteResults(route) {
  const box = document.getElementById("route-results-box");
  if (!box) return;
  box.classList.remove("hidden");

  document.getElementById("res-algo-name").textContent =
    route.algorithm === "fastest" ? "Fastest Route (Time Weighted)" :
    route.algorithm === "bellman_ford" ? "Bellman-Ford Algorithm" : "Dijkstra (Shortest Distance)";

  document.getElementById("res-distance").textContent = `${route.total_distance.toFixed(2)} km`;
  document.getElementById("res-time").textContent = `${route.total_time.toFixed(2)} hrs (${route.time_formatted || ""})`;
  document.getElementById("res-complexity").textContent = `${route.time_complexity || ""} / Space ${route.space_complexity || ""}`;
  document.getElementById("res-exec-time").textContent = `${(route.execution_time_ms || 0.012).toFixed(4)} ms`;

  // Path Nodes
  const pathContainer = document.getElementById("res-path-list");
  if (pathContainer && route.path_names) {
    pathContainer.innerHTML = route.path_names
      .map((name, idx) => `
        <div class="path-step-node">
          <div class="path-step-idx">${idx + 1}</div>
          <div class="path-step-name">${name}</div>
        </div>
      `)
      .join("");
  }

  // Stations En Route
  const stationsBox = document.getElementById("res-stations-on-route");
  if (stationsBox) {
    if (route.stations_on_route && route.stations_on_route.length > 0) {
      stationsBox.innerHTML = route.stations_on_route
        .map((s) => `
          <div class="station-item" style="border-left: 3px solid #00f0ff;">
            <div><b>⚡ ${s.name}</b> (${s.chargers} chargers)</div>
            <div style="color: #38bdf8; font-size: 0.75rem;">₹${s.cost}</div>
          </div>
        `)
        .join("");
    } else {
      stationsBox.innerHTML = `<div style="font-size: 0.78rem; color: #94a3b8; font-style: italic;">No charging stations installed along this route.</div>`;
    }
  }

  // Detailed Hops Table
  const hopsTable = document.getElementById("res-hops-table-body");
  if (hopsTable && route.hops) {
    hopsTable.innerHTML = route.hops
      .map((hop, i) => `
        <tr>
          <td>${i + 1}</td>
          <td>${hop.from_name} → ${hop.to_name}</td>
          <td><b>${hop.distance.toFixed(2)} km</b></td>
          <td>${hop.time.toFixed(2)} hrs</td>
          <td>${(hop.distance / hop.time).toFixed(1)} km/h</td>
        </tr>
      `)
      .join("");
  }
}

// Strategy Comparison Handler
async function runStrategyComparison() {
  const budget = parseFloat(document.getElementById("strat-budget").value);
  const cost = parseFloat(document.getElementById("strat-cost").value);
  const chargers = parseInt(document.getElementById("strat-chargers").value);
  const maxCap = parseInt(document.getElementById("strat-max-cap").value);

  const demandInputs = document.querySelectorAll(".demand-score-input");
  const demandScores = {};
  demandInputs.forEach((inp) => {
    demandScores[inp.name] = parseFloat(inp.value) || 50.0;
  });

  const payload = {
    budget,
    station_cost: cost,
    chargers_per_station: chargers,
    maximum_chargers: maxCap,
    ...demandScores,
  };

  try {
    const res = await fetch(`${API_BASE}/api/compare_strategies`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });
    const data = await res.json();
    if (!data.success) {
      showNotification(data.error || "Comparison failed", "error");
      return;
    }

    renderStrategyResults(data);
    showNotification("Strategy Comparison calculated successfully via C backend!", "success");
  } catch (err) {
    showNotification("Failed to run strategy comparison", "error");
  }
}

function renderStrategyResults(d) {
  document.getElementById("strat-res-container")?.classList.remove("hidden");

  // Strategy A
  const a = d.strategy_a;
  document.getElementById("strat-a-count").textContent = a.count;
  document.getElementById("strat-a-chargers").textContent = a.total_chargers;
  document.getElementById("strat-a-cost").textContent = `₹${a.total_cost.toFixed(2)}`;
  document.getElementById("strat-a-score").textContent = a.captured_score.toFixed(1);

  const listA = document.getElementById("strat-a-stations");
  if (listA) {
    listA.innerHTML = (a.stations || [])
      .map((s) => `
        <li class="station-item">
          <span><b>${s.name}</b> (${s.connections} roads)</span>
          <span style="color: #38bdf8;">Priority: ${s.demand_score}</span>
        </li>
      `)
      .join("");
  }

  // Strategy B
  const b = d.strategy_b;
  document.getElementById("strat-b-count").textContent = b.count;
  document.getElementById("strat-b-chargers").textContent = b.total_chargers;
  document.getElementById("strat-b-cost").textContent = `₹${b.total_cost.toFixed(2)}`;
  document.getElementById("strat-b-score").textContent = b.captured_score.toFixed(1);

  const listB = document.getElementById("strat-b-stations");
  if (listB) {
    listB.innerHTML = (b.stations || [])
      .map((s) => `
        <li class="station-item">
          <span><b>${s.name}</b> (${s.connections} roads)</span>
          <span style="color: #10b981;">Priority: ${s.demand_score}</span>
        </li>
      `)
      .join("");
  }
}

// Populate Demand Table
function populateDemandTable() {
  if (!networkData || !networkData.nodes) return;
  const tbody = document.getElementById("demand-table-body");
  if (!tbody) return;

  const defaultScores = [95, 90, 85, 60, 88, 70, 82, 75, 78, 68];

  tbody.innerHTML = networkData.nodes
    .map((n, i) => `
      <tr>
        <td><b>${n.id}</b></td>
        <td>${n.name}</td>
        <td><span class="tag">${n.edge_count} roads</span></td>
        <td>
          <input type="number" step="0.5" min="1" max="100" class="form-control demand-score-input" name="demand_${n.id}" value="${i < 10 ? defaultScores[i] : 50}" style="width: 80px; padding: 3px 6px;">
        </td>
      </tr>
    `)
    .join("");
}

// Installed Stations Table
function updateInstalledStationsTable() {
  if (!networkData || !networkData.nodes) return;
  const tbody = document.getElementById("installed-stations-body");
  if (!tbody) return;

  const installed = networkData.nodes.filter((n) => n.has_station);

  if (installed.length === 0) {
    tbody.innerHTML = `<tr><td colspan="5" style="text-align:center; color: #94a3b8; padding: 16px;">No charging stations installed yet. Use the manual form or automatic greedy placement above.</td></tr>`;
    return;
  }

  tbody.innerHTML = installed
    .map((n) => `
      <tr>
        <td><b>${n.id}</b></td>
        <td>⚡ ${n.name}</td>
        <td>${n.charger_count} Bays</td>
        <td>₹${n.installation_cost.toFixed(2)}</td>
        <td><span class="tag" style="background: rgba(16, 185, 129, 0.2); color: #10b981;">Active</span></td>
      </tr>
    `)
    .join("");
}

// Sorting & Searching Visualizer
async function runSortingAndSearch() {
  const rawArray = document.getElementById("sort-array-input")?.value || "45, 12, 78, 23, 9, 56, 34, 89";
  const key = parseInt(document.getElementById("search-key-input")?.value || "23");
  const sortAlgo = document.getElementById("sort-algo-select")?.value || "quick";
  const searchAlgo = document.getElementById("search-algo-select")?.value || "iterative";

  const arr = rawArray
    .split(",")
    .map((x) => parseInt(x.trim()))
    .filter((x) => !isNaN(x));

  if (arr.length === 0) {
    showNotification("Please provide a valid comma-separated array of integers.", "warning");
    return;
  }

  try {
    const res = await fetch(`${API_BASE}/api/sort_search`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        array: arr,
        key: key,
        sort_algo: sortAlgo,
        search_algo: searchAlgo,
      }),
    });

    const data = await res.json();
    if (!data.success) {
      showNotification("Sort & Search execution failed", "error");
      return;
    }

    renderSortSearchResults(data);
    showNotification(`${sortAlgo.toUpperCase()} & ${searchAlgo.toUpperCase()} Binary Search executed in ${data.sort_time_us.toFixed(2)} µs!`, "success");
  } catch (err) {
    showNotification("Failed to run sort/search on C backend", "error");
  }
}

function renderSortSearchResults(data) {
  const container = document.getElementById("sort-results-card");
  if (!container) return;
  container.classList.remove("hidden");

  document.getElementById("sort-algo-badge").textContent = `${data.sort_algo.toUpperCase()} SORT`;
  document.getElementById("sort-complexity-badge").textContent = data.sort_complexity;
  document.getElementById("sort-exec-time").textContent = `${data.sort_time_us.toFixed(2)} µs`;

  // Render Visualizer Bars
  const barsContainer = document.getElementById("array-bars-container");
  if (barsContainer) {
    const maxVal = Math.max(...data.sorted, 1);
    barsContainer.innerHTML = data.sorted
      .map((val, idx) => {
        const heightPct = Math.max((val / maxVal) * 100, 10);
        const isMatched = idx === data.search_index;
        return `
          <div class="array-bar-wrap">
            <div class="array-bar ${isMatched ? "matched" : ""}" style="height: ${heightPct}%;"></div>
            <div class="array-bar-val ${isMatched ? "style='color:#10b981;'" : ""}">${val}</div>
          </div>
        `;
      })
      .join("");
  }

  // Search Results
  const resIdxEl = document.getElementById("search-res-index");
  if (resIdxEl) {
    if (data.search_index !== -1) {
      resIdxEl.innerHTML = `<span style="color: #10b981; font-weight: 700;">Key ${data.key} found at sorted index ${data.search_index}</span>`;
    } else {
      resIdxEl.innerHTML = `<span style="color: #f43f5e; font-weight: 700;">Key ${data.key} not found in array.</span>`;
    }
  }
}

// Live Stress Benchmark Runner
async function runLiveStressBenchmark() {
  const repCount = parseInt(document.getElementById("stress-reps-input")?.value || "5000");
  const btn = document.getElementById("btn-run-stress-test");
  const spinner = document.getElementById("stress-spinner");

  if (btn) btn.disabled = true;
  if (spinner) spinner.classList.remove("hidden");

  try {
    const res = await fetch(`${API_BASE}/api/stress_benchmark`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ repetitions: repCount }),
    });

    const data = await res.json();
    if (!data.success) {
      showNotification("Benchmark execution failed", "error");
      return;
    }

    renderStressBenchmarkResults(data);
    showNotification(`Stress test with ${repCount} iterations completed successfully!`, "success");
  } catch (err) {
    showNotification("Failed to run stress benchmark on C backend", "error");
  } finally {
    if (btn) btn.disabled = false;
    if (spinner) spinner.classList.add("hidden");
  }
}

function renderStressBenchmarkResults(d) {
  document.getElementById("stress-res-container")?.classList.remove("hidden");

  document.getElementById("bench-nodes").textContent = d.node_count;
  document.getElementById("bench-edges").textContent = d.edge_count;
  document.getElementById("bench-reps").textContent = d.repetitions.toLocaleString();

  document.getElementById("bench-dijkstra-time").textContent = `${d.dijkstra_time_ms.toFixed(6)} ms`;
  document.getElementById("bench-bellman-time").textContent = `${d.bellman_time_ms.toFixed(6)} ms`;

  const matchEl = document.getElementById("bench-correctness");
  if (matchEl) {
    if (d.distances_match) {
      matchEl.innerHTML = `<span class="pass-tag">✓ PASS: Distances Match 100%</span>`;
    } else {
      matchEl.innerHTML = `<span style="color: #f43f5e; font-weight: 700;">✗ FAIL: Distances Mismatched</span>`;
    }
  }

  document.getElementById("bench-mem-graph").textContent = `${d.graph_memory_bytes.toLocaleString()} bytes`;
  document.getElementById("bench-mem-dist").textContent = `${d.distance_arrays_bytes.toLocaleString()} bytes`;
  document.getElementById("bench-mem-visited").textContent = `${d.dijkstra_visited_bytes.toLocaleString()} bytes`;
}

// Notification Toast
function showNotification(msg, type = "info") {
  const toast = document.createElement("div");
  toast.className = `toast toast-${type}`;
  toast.style.cssText = `
    position: fixed;
    bottom: 24px;
    right: 24px;
    padding: 12px 18px;
    border-radius: 8px;
    background: ${type === "error" ? "rgba(244, 63, 94, 0.95)" : type === "success" ? "rgba(16, 185, 129, 0.95)" : type === "warning" ? "rgba(245, 158, 11, 0.95)" : "rgba(0, 112, 243, 0.95)"};
    color: #fff;
    font-size: 0.82rem;
    font-weight: 600;
    box-shadow: 0 8px 25px rgba(0,0,0,0.5);
    backdrop-filter: blur(8px);
    z-index: 9999;
    animation: fadeIn 0.25s ease;
  `;
  toast.textContent = msg;
  document.body.appendChild(toast);

  setTimeout(() => {
    toast.style.opacity = "0";
    toast.style.transition = "opacity 0.3s ease";
    setTimeout(() => toast.remove(), 300);
  }, 4000);
}
