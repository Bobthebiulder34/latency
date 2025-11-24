// live-graph.js
// Fetches `sample.csv`, parses CPU/GPU/SSD rows and draws a Chart.js line chart.
// It polls the CSV every 5 minutes and appends any new samples so the graph "grows".

const POLL_INTERVAL_MS = 5 * 60 * 1000; // 5 minutes
let chart = null;

async function fetchCSV() {
    // cache-bust so browsers don't return a cached CSV
    const res = await fetch(`sample.csv?_=${Date.now()}`);
    if (!res.ok) throw new Error('Failed to fetch sample.csv: ' + res.status);
    const text = await res.text();
    return parseCSV(text);
}

function parseCSV(text) {
    const lines = text.trim().split(/\r?\n/).filter(Boolean);
    if (lines.length < 2) return { labels: [], metrics: {} };
    const header = lines[0].split(',').map(s => s.trim());
    const sampleCount = header.length - 1;
    // Label samples as '5 minutes', '10 minutes', ...
    const labels = Array.from({ length: sampleCount }, (_, i) => `${(i + 1) * 5} minutes`);
    const metrics = {};
    for (let i = 1; i < lines.length; i++) {
        const parts = lines[i].split(',').map(s => s.trim());
        const key = parts[0].toLowerCase();
        metrics[key] = parts.slice(1).map(v => {
            const n = Number(v);
            return Number.isFinite(n) ? n : null;
        });
    }
    return { labels, metrics };
}

function buildDatasets(metrics) {
    return [
        {
            label: 'CPU',
            data: metrics.cpu || [],
            borderColor: 'rgb(220,53,69)',
            backgroundColor: 'rgba(220,53,69,0.2)',
            tension: 0.2,
            fill: false,
        },
        {
            label: 'GPU',
            data: metrics.gpu || [],
            borderColor: 'rgb(23,162,184)',
            backgroundColor: 'rgba(23,162,184,0.2)',
            tension: 0.2,
            fill: false,
        },
        {
            label: 'SSD',
            data: metrics.ssd || [],
            borderColor: 'rgb(40,167,69)',
            backgroundColor: 'rgba(40,167,69,0.2)',
            tension: 0.2,
            fill: false,
        }
    ];
}

async function initChart() {
    try {
        const { labels, metrics } = await fetchCSV();
        const ctx = document.getElementById('usageChart').getContext('2d');
        chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels,
                datasets: buildDatasets(metrics)
            },
            options: {
                responsive: true,
                animation: false,
                plugins: {
                    legend: { position: 'top' }
                },
                scales: {
                    y: { beginAtZero: true }
                }
            }
        });

        // start polling
        setInterval(pollAndAppend, POLL_INTERVAL_MS);
    } catch (err) {
        console.error('Failed to initialize chart:', err);
        const el = document.getElementById('particle-output');
        if (el) el.textContent = 'Error initializing graph. See console.';
    }
}

async function pollAndAppend() {
    if (!chart) return;
    try {
        const { labels, metrics } = await fetchCSV();
        // if there are more labels than chart, append the new ones
        if (labels.length > chart.data.labels.length) {
            for (let i = chart.data.labels.length; i < labels.length; i++) {
                chart.data.labels.push(labels[i]);
            }
            // append dataset values
            const keys = ['cpu', 'gpu', 'ssd'];
            for (let di = 0; di < chart.data.datasets.length; di++) {
                const key = keys[di];
                const arr = metrics[key] || [];
                for (let i = chart.data.datasets[di].data.length; i < arr.length; i++) {
                    chart.data.datasets[di].data.push(arr[i]);
                }
            }
            chart.update();
        }
    } catch (err) {
        console.error('Polling failed:', err);
    }
}

// initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initChart);
} else {
    initChart();
}
